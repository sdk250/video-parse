# -*- coding: UTF-8 -*-
from flask import Flask, request
import requests
import json
import re

class parse(object):
	def __init__(self):
		self.user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.5060.114 Safari/537.36 Edg/103.0.1264.62"
		self.accept = "Application/json; charet: UTF-8"
		self.connection = "Close"

	def pipix(self, id):
		id_ = re.compile("(/{1}s/{1})([0-9a-zA-Z]{5,})").findall(id)
		response = requests.get(
			url = "https://h5.pipix.com/s/" + id_[0][1],
			headers = {
				"Host": "h5.pipix.com",
				"User-Agent": self.user_agent,
				"Accept": self.accept,
				"Connection": self.connection
			},
			allow_redirects = False
		)
		id_ = re.compile("(/{1}item/{1})([0-9]{10,})").findall(response.text)
		response = json.loads(requests.get(
			url = "https://is.snssdk.com/bds/cell/detail/?cell_type=1&aid=1319&app_name=super&cell_id=" + id_[0][1],
			headers = {
				"Host": "is.snssdk.com",
				"User-Agent": self.user_agent,
				"Accept": self.accept,
				"Connection": self.connection
			},
			allow_redirects = False
		).text)
		god_comment = None
		if response["data"]["data"]["item"]["comments"]:
			god_comment = response["data"]["data"]["item"]["comments"][0]["text"]
		return {
			"message": "success",
			"data": {
				"title": response["data"]["data"]["item"]["content"],
				"god-comment": god_comment,
				"url": response["data"]["data"]["item"]["video"]["video_high"]["url_list"][0]["url"]
			}
		}

	def kuaishou(self, id):
		id_ = re.compile("((http|https)?://v.(kuaishou|kuaishouapp)?\.com/.{3,6}[a-z0-9A-Z]+)").findall(id)
		print(id_)
		response = requests.get(
			url = id_[0][0],
			headers = {
				"Host": "v.kuaishou.com",
				"User-Agent": self.user_agent,
				"Accept": self.accept,
				"Connection": self.connection
			},
			allow_redirects = False
		)
		print(response.headers)
		"""
		id_ = re.compile("photoId\=([0-9a-zA-Z]{10,})").findall(response.headers["Location"])
		response = json.loads(requests.post(
			url = "https://v.m.chenzhongtech.com/rest/wd/photo/info",
			data = json.dumps({
				"photoId": str(id_[0]),
				"isLongVideo": "false"
			}),
			headers = {
				"Host": "v.m.chenzhongtech.com",
				"User-Agent": self.user_agent,
				"Accept": self.accept,
				"Connection": "Keep-Alive",
				"Content-Type": "application/json",
				"Origin": "https://www.kuaishou.com",
				"Referer": response.headers["Location"],
				"Cookie": "did=web_5f1471502c301bfca2385b8de925aa69; didv=1658645570591;"
			},
			allow_redirects = False
		).text)
		return {
			"message": "success",
			"data": {
				"title": response["shareInfo"]["shareTitle"],
				"god-comment": None,
				"url": response["photo"]["mainMvUrls"][0]["url"]
			}
		}
		"""

	def run(self):
		# a = self.pipix("https://h5.pipix.com/s/251aans/# [皮皮虾] 孤独就像这个圈圈 【长按复制】到浏览器观看")
		b = self.kuaishou("https://v.kuaishouapp.com/s/QKtYIfi3 卢卢溜溜球 \"可莉 \"仲夏幻夜奇想曲 \"原神 复制此消息，打开【快手极速版】直接观看！")
		b = self.kuaishou("https://v.kuaishou.com/qZgvoL 你以为元歌那么好玩吗？我也来试试针对元歌")
		# print("Video title:", a["data"]["title"])
		# print("Video god comment:", a["data"]["god-comment"])
		# print("Video url:", a["data"]["url"])
		print("====\t====")
		print("Video title:", b["data"]["title"])
		print("Video url:", b["data"]["url"])

app = Flask(__name__)
sdk250 = parse()

@app.route("/", methods = ["GET", "POST"])
def index():
	if request.args.get("url"):
		if re.findall(re.compile("(kuaishou|kuaishouapp)"), request.args.get("url")) != []:
			return sdk250.kuaishou(request.args.get("url"))
		elif re.findall(re.compile("(pipix)"), request.args.get("url")) != []:
			return sdk250.pipix(request.args.get("url"))
		else:
			return {
				"code": 404,
				"message": "Null"
			}
	else:
		return "URL is Null.\n"

if __name__ == "__main__":
	# app.run(host = "0.0.0.0", port = 20880, debug = True)
	sdk250.kuaishou("https://v.kuaishouapp.com/s/2W6Im5AC 说陈道长不是修仙这是说不过去的\"陈道长 \"捕娱计划 \"娱你星动计划第一期 该作品在快手被播放过772.3万次，点击链接，打开【快手极速版】直接观看！")
