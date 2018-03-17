<h1>Flib v1.1</h1>
<p align="center"><b>====<img src="https://rorytrotter.files.wordpress.com/2013/03/red-circle.jpg" width="17"/>====</b></p>
<h2>Description</h2>

Classes for managing windows and creating OpenGL contexts (for Windows and Linux)
Automatic OpenGL functions loading
Wrapper classes for OpenGL objects (Texture, FrameBuffer, Shader etc)
Template classes (Vectors, Matrices, Polar vectors, Delegates)

<p align="center"><b>====<img src="https://rorytrotter.files.wordpress.com/2013/03/red-circle.jpg" width="17"/>====</b></p>
<h2>Compilation</h2>

The library requires OpenGL development library to compile
On Linux systems this means you need glx development library (e.g. from `mesa-common-dev`)<br>
A `makefile` is loacted in `src/`
Open a terminal in `src/` and call make

<p align="center"><b>====<img src="https://rorytrotter.files.wordpress.com/2013/03/red-circle.jpg" width="17"/>====</b></p>
<h2>Troubleshooting</h2>

When compiling on Linux systems the following errors may arise

| Symptom | Cause | Solution | Command |
| ------- |:-----:|:--------:| -------:|
| GL/glx.h not found | glx not installed | Install glx | `sudo apt-get install mesa-common-dev` |
| libGL.so not found | OpenGL not updated | Install/Update OpenGL | `sudo apt-get install libgl1-mesa-dev` or/then `sudo ldconfig` |

Please note that the actual command may differ on your system,
refer to your distribution's forum how to install the required
development libraries

<p align="center"><b>====<img src="https://rorytrotter.files.wordpress.com/2013/03/red-circle.jpg" width="17"/>====</b></p>
<h2>API Documentation</h2>

The source code uses selfdocumentation in doxygen style
A compiled html version can be found on my site:
http://makom789.web.elte.hu/docs/index.html
<p align="center"><b>====<img src="https://rorytrotter.files.wordpress.com/2013/03/red-circle.jpg" width="17"/>====</b></p>
<h2>Automatic testing</h2>
Testing done via AutoTester (https://github.com/Frontier789/AutoTester)
<p align="center"><img src="https://raw.githubusercontent.com/Frontier789/AutoTester/master/result.png"/></p>

<p align="center"><b>====<img src="https://rorytrotter.files.wordpress.com/2013/03/red-circle.jpg" width="17"/>====</b></p>
<h2>Citation</h2>
The source uses two public domain libraries:
<ol>
	<li>
		<p>stb (http://github.com/nothings/stb and http://nothings.org)</p>
		<ul type="*">
			<li>stb_image.h</li>
		</ul>
		<ul type="*">
			<li>stb_image_write.h</li>
		</ul>
		<ul type="*">
			<li>stb_image_resize.h</li>
		</ul>
		<ul type="*">
			<li>stb_truetype.h</li>
		</ul>
	</li>
	<li>
		<p>jpeg (https://code.google.com/p/jpeg-compressor/)</p>
		<ul type="*">
			<li>jpge.h</li>
			<li>jpge.cpp</li>
		</ul>
	</li>
</ol>
<p align="center"><b>====<img src="https://rorytrotter.files.wordpress.com/2013/03/red-circle.jpg" width="17"/>====</b></p>