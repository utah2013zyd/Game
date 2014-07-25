
1.首先要确定你按照《第一周 星期天》课程中地方法构建并安装了Orz SDK。    
2.通过‘开始’>‘所有程序’>‘CMake 2.8’>‘CMake’来运行CMake。
3.在"Where is the source code"文本框中，输入或者“Browse Source...”到例子源代码的根目录(在OgreBook/Example/MVC，即是包含Controller等文件夹的文件夹)。
4.在"Where to build the binaries" 文本框中，输入或者“Browse Build...”到任何你喜欢的文件夹中——它会被用于输出构建结果（lib文件，头文件，dll文件以及执行程序。如果没有这个文件夹系统会帮你创建。注意，你最好按照需要生成到另外的文件夹中，而不是源代码目录下。
5.单击屏幕下方的'Configure'按键。
6.选择你的目标编译平台，例如"Visual Studio 9 2008"。
7.当系统提示是否创建构建目录的时候选择'Ok'。
8.等待Configure执行结束。
9.屏幕上现在会有一些配置设置，并用红色标记（这是用来指出你是第一次看到他们）。你可以在这里手动配置这些选项，不过现在我们只需要再次选择'Configure'。
10.等运行结束了选择"Generate"按键。
11.构建文件将会在你之前选择的文件夹下面产生，这时候就可以退出CMake来进行真正的编译工作。
12.在你之前选择的文件夹中，你会发现有一系列工程文件。找到并打开主项目文件MVC_Example.sln。
13.分别在Debug和Release环境下，构建ALL_BUILD项目，之后系统会依次构建所有工程。
14.分别在Debug和Release环境下，鼠标右键选择INSTALL项目，在目录中选择“Project Only > Build Only INSTALL”，系统会安装整个项目。
15.游戏会被安装到"OgreBook/Build/vc90/MVC_Example"（在使用VC9.0的情况下）下面，里面包含bin、lib以及media三个文件夹。
16.分别进入"OgreBook/Build/vc90/MVC_Example/bin/Release"以及"OgreBook/Build/vc90/MVC_Example/bin/Debug"两个目录，里面包含例程生成的文件。