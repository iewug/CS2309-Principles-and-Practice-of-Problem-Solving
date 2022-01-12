# README

<p style="text-align:right;">TomatoEater<br>2021/12</p>

*写在前面*

CS2309秋季问题求解与实践英文班大作业。老师是赵杰茹、金海明。前1-12周上程序设计、数据结构、神经网络、qt。前期课程量较可，每周有编程或口胡作业。我们有一次课堂小测，是分析散列表时间复杂性，没有课堂点名。后面四周不上课，自己写大作业，大作业量较大。每年大作业会变。我们这届写的是POI checking-in data的可视化。最近这两届qt大作业如果要做选做都要用qml来实现map。本人对此不作评价。

希望能对学弟学妹有所帮助。

***



*正文*

This is an app for data analysis and visualization of POI checking-in data, which is also the final project of SJTU CS2309@Fall 2021 (Principles and Practice of Problem Solving).

Here is a README file to help you run the code. For more detail, refer to the `report.pdf`.

## 1. How to Build

1. Open the `finalproject.pro` in Qt Creator
2. Chose Desktop Qt 5.15.2 MinGW 64-bit (or other Qt environment less than 5.16 in your computer)
3. Run the project

The code is only tested under Windows10 enviroment, but it should work well in MacOS or Linux.

### or just Running the exe File

I also provide the final released version for my program. If you are under Windows10 enviroment, feel free to open the `release` directory and run the `exe` file.

## 2. How to Use

There is a navigation bar above. Before trying any function, please turn to the `Filter` page first, and load the csv file. Otherwise, a warning will be shown to told you open the file first.

### 2.1 Filter

Any combination of checkboxes is acceptable.

<img src="https://github.com/tomatoeater/CS2309-Principles-and-Practice-of-Problem-Solving/raw/main/picForREADME/filter.png" style="zoom:60%;" />

### 2.2 User's Favorite

Be careful about typing userIDs. But I do input check to inform you when your input goes wrong.

<img src="https://github.com/tomatoeater/CS2309-Principles-and-Practice-of-Problem-Solving/raw/main/picForREADME/users' fav.png" style="zoom:60%;" />

### 2.3 Checking-in Number

Also be careful when typing POIIDs. You should select only one checkbox between POI_ID and GPS Range. You should select only one checkbox between by Month and by Hour. 

<img src="https://github.com/tomatoeater/CS2309-Principles-and-Practice-of-Problem-Solving/raw/main/picForREADME/chk over time.png" style="zoom:60%;" />

### 2.4 DAU

When there is no data available, both lines will be 0.

<img src="https://github.com/tomatoeater/CS2309-Principles-and-Practice-of-Problem-Solving/raw/main/picForREADME/dau.png" style="zoom:60%;" />

### 2.5 User's Diff

When there is no data available, a warning window will be shown.

<img src="https://github.com/tomatoeater/CS2309-Principles-and-Practice-of-Problem-Solving/raw/main/picForREADME/user dif.png" style="zoom:60%;" />

### 2.6 Trajectory

When there is no data available, a warning window will be shown.

<img src="https://github.com/tomatoeater/CS2309-Principles-and-Practice-of-Problem-Solving/raw/main/picForREADME/trajectory.png" style="zoom:60%;" />

### 2.7 Heatmap

You should select only one checkbox between POI_ID and GPS Range.

<img src="https://github.com/tomatoeater/CS2309-Principles-and-Practice-of-Problem-Solving/raw/main/picForREADME/heatmap.png" style="zoom:60%;" />

### 2.8 Prediction

You should select only one checkbox between POIID and UserID.

<img src="https://github.com/tomatoeater/CS2309-Principles-and-Practice-of-Problem-Solving/raw/main/picForREADME/prediction.png" style="zoom:60%;" />

## 3. BUT

Feel free to explore by yourself! The program is well designed and hopefully will inform you when input is wrong.

