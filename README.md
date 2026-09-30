# Screen-Jumper
Little guy that can jump around on your screen, using the horizontal lines that appear on your screen as platforms.

### A Little Note, as of Right Now
I've been having some trouble with some of the rather essential operations for this project
* taking screenshot/screen recording
* not being able to get keyboard input when the game window's not in focus since that's technically key-logging, lol, and Wayland doesn't like that
* the edge detection is very iffy and can generate too many lines or inconsistent/incomplete ones

I think there's some easier solutions for some of key logging issues on Windows, so maybe I'll come back to this and try to focus on developing it for Windows first, and push Linux support to later.

But for now, I think I'm going to shift to making this a Chrome extension instead, because then focus won't be an issue since it's already on top of the active window, and maybe I can try different approaches to getting the platforms (like getting the actual UI elements on the screen and their dimensions) rather than purely image edge detection, which an be finicky. Here's a link to the repo that Screen Jumper (Chrome Extension) will be on: https://github.com/QuentinRivest/Screen-Jumper-Chrome-Extension.
