---
date: 2025-02-04 19:48:00 +0800
redirect_from:
  - /README/
permalink: /
---

# CBJQ_XiaoKaiGuan

尘白禁区 *XiaoKaiGuan*（小开关😎😋）

这个项目很简单，只是为了方便不那么熟悉Windows系统各种操作的玩家直接开关小开关使用。

<div style="align-items: center; justify-content: center; display: flex; margin: 10px;">
   <img src="res\icons_pic\芬妮-辉耀 团子.png" style=" max-height: 300px; max-width:75%; aspect-ratio: 1;">
</div>

<div style="align-items: center; justify-content: center; display: flex; margin: 10px; gap: 10px">
   <img alt="GitHub Downloads (all assets, all releases)" src="https://img.shields.io/github/downloads/LiuJiewenTT/CBJQ_XiaoKaiGuan/total">
   <img alt="GitHub Release" src="https://img.shields.io/github/v/release/LiuJiewenTT/CBJQ_XiaoKaiGuan">
   <img alt="GitHub Actions Workflow Status" src="https://img.shields.io/github/actions/workflow/status/LiuJiewenTT/CBJQ_XiaoKaiGuan/jekyll-gh-pages.yml">
   <img alt="GitHub License" src="https://img.shields.io/github/license/LiuJiewenTT/CBJQ_XiaoKaiGuan">
</div>

项目地址：[https://github.com/LiuJiewenTT/CBJQ_XiaoKaiGuan](https://github.com/LiuJiewenTT/CBJQ_XiaoKaiGuan)<br>
项目主页：[https://LiuJiewenTT.github.io/CBJQ_XiaoKaiGuan](https://LiuJiewenTT.github.io/CBJQ_XiaoKaiGuan)<br>
下载页：[https://github.com/LiuJiewenTT/CBJQ_XiaoKaiGuan/releases](https://github.com/LiuJiewenTT/CBJQ_XiaoKaiGuan/releases)<br>

## 项目历史

前有更加简单的实现，即bat脚本，但无法加上图标，不那么美观。原先在作者的[Gist中](https://gist.github.com/LiuJiewenTT/ef6966810da80200fce8a3e8ff13e69e)有一份，叫做[`Enya_Switch`](alters/Enya_Switch/README.md)，启动和结束都是单独的脚本。也就是说，如果创建快捷方式到桌面，可能需要两个快捷方式。另外，原脚本需要玩家手动附上管理员权限，不方便。

## 项目介绍

这个项目使用C语言实现，提供开和关的统一管理入口，并展示图标。

> 项目也提供其他版本的实现，比如bat脚本的实现。

> 注：由于作者不是很确定用什么图标，以及图标原图的作者是谁，相关的版权要求如何，所以请各位帮帮忙，帮忙找些好看的图，也帮作者找找图片资源作者，谢谢！
>
> 注：项目作者开展此项目单纯是出于兴趣爱好，以及帮助伙伴的热心，无意冒犯、无意侵权，如果有什么问题，请立即联系作者，作者会及时处理，包括但不限于删除、修改、下架、发表道歉声明等。还望各位理解，作者无意冒犯。
>
> 注：暂未获得作者授权的图标，不会公开在代码库中，仅提供临时构建成品，且成品随时会应要求下架。

## 如何使用？

1. 把程序放在`localization.txt`所在的目录下。
2. 双击启动或者右键“以管理员身份运行”。

> 注：多次启动会展示程序信息。

## 程序特性

1. 可以运行期间**无窗口后台**运行。

2. 支持双运行模式：控制台显示运行（模式一）和无窗口后台运行（模式二）。便于调试。

3. 无窗口模式自动记录日志到文件，便于查错。

   > 切换到有窗口模式需要添加程序名+`.unhide`文件。例如：程序*CBJQ_XiaoKaiGuan.exe*对应的是*CBJQ_XiaoKaiGuan.exe<span style="color:green">.unhide</span>*

## Copyrights

此部分主要是关于资源的版权声明。（排序不分先后）

| 资源位置           | 版权所有者                                   | 备注                                           |
| ------------------ | -------------------------------------------- | ---------------------------------------------- |
| 芬妮-辉耀 团子.png | *魔法少女鱼鱼熏Kaori* (B站用户UID: 66874794) |                                                |
| icon1.ico          | *魔法少女鱼鱼熏Kaori* (B站用户UID: 66874794) | 本项目作者自行从`芬妮-辉耀 团子.png`转换而来。 |

