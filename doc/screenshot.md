## how to take screen shot on a debian

### record window
```
❯ xwininfo | grep 'Window id' | awk '{print "recordmydesktop", "--no-sound", "--overwrite", "--windowid", $4}' | sh
```

### convert to gif
```
❯ ffmpeg -y -i out.ogv -pix_fmt rgb24 screen.gif
```
