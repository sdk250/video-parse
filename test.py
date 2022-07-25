import re

a = "https://v.kuaishou.com/qfriqz 最后一波！队友只要一息尚存 哪怕敌力胜我数倍 我亦不怕！ \"王者荣耀 \"格局 \"永珏导师 该作品在快手被播放过2,040.9万次，点击链接，打开【快手】直接观看！"
b = "https://v.kuaishouapp.com/s/QKtYIfi3 卢卢溜溜球 \"可莉 \"仲夏幻夜奇想曲 \"原神 复制此消息，打开【快手极速版】直接观看！"
c = "https://h5.pipix.com/s/2HQJVqr/# [皮皮虾] 天王老子来了也不行 【长按复制】到浏览器观看"

def index(str = None):
    if str:
        d = re.findall(re.compile("(kuaishou|kuaishouapp)"), str)
        print(d != [])
    else:
        print("Not str.\n")

if __name__ == "__main__":
    index(a)
    index()
    index(b)
    index(c)
