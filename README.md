<h1>Flib v0.8</h1>
<p align="center"><b>====<img src="https://photos-2.dropbox.com/t/2/AACLQ8QkdIkZc5JkDlZqYdZjT_SiLHMgBZEw74lmsUsanQ/12/146126124/png/32x32/1/_/1/2/circle.png/EPCo528YlV4gAigC/X6T3yLuf07n4PX7i64kLn63qmi2YQFlo8-SSdj6fmQ4?size=1280x960&size_mode=3"/>====</b></p>
<h2>Description</h2>

Classes for managing windows and creating OpenGL contexts (for Windows and Linux)
Automatic OpenGL functions loading
Wrapper classes for OpenGL objects (Texture, FrameBuffer, Shader etc)
Templated math classes (Vector, Matrix)

<p align="center"><b>====:o:====</b></p>
<h2>Compilation</h2>

cd to /src

|    OS   |     Invoke command     |
|:-------:|:----------------------:|
| Windows |  make -f makefile.win  |
|  Linux  | make -f makefile.linux |
| Android |make -f makefile.android|

<p align="center"><b>====:o:====</b></p>
<h2>Citation</h2>
The source uses two public domain libraries and a GPLv2 library:
<ol>
	<li>
		<p>stb (http://github.com/nothings/stb and http://nothings.org)</p>
		<ul type="*">
			<li>stb_image.h</li>
		</ul>
	</li>
	<li>
		<p>jpeg (https://code.google.com/p/jpeg-compressor/)</p>
		<ul type="*">
			<li>jpge.h</li>
			<li>jpge.cpp</li>
		</ul>
	</li>
	<li>
		<p>slightly modified version of freetype (http://www.freetype.org/)</p>
		<ul type="*">
			<li>src/Graphics/FreeType/*</li>
		</ul>
	</li>
</ol>
<p align="center"><b>====:o:====</b></p>