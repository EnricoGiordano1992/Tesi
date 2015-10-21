rem The following converts Targa 256-color files into hex files to include in code.
rem Small Size must be 64x64. (large size 128x128) Save WITHOUT Run Length Encoding.
hex2code CROSS.TGA G_calibrationIcon "const TUInt8" >CalibrationIcon.h
hex2code REEL2.TGA G_slideshowIcon "const TUInt8" >SlideshowIcon.h
hex2code TERMINAL.TGA G_consoleIcon "const TUInt8" >ConsoleIcon.h
hex2code PALETTE.TGA G_drawIcon "const TUInt8" >DrawIcon.h
hex2code load8bit.TGA G_loadIcon "const TUInt8" >LoadIcon.h
hex2code save8bit.TGA G_saveIcon "const TUInt8" >SaveIcon.h
hex2code leftarrow.TGA G_exitIcon "const TUInt8" >ExitIcon.h
hex2code title.TGA G_titleScreen "const TUInt8" >TitleScreen.h
hex2code ACCEL.TGA G_accelIcon "const TUInt8" >AccelIcon.h
hex2code SETTINGS.TGA G_settingsIcon "const TUInt8" >SettingsIcon.h
hex2code TIMETEMP.TGA G_timeTempIcon "const TUInt8" >TimeTempIcon.h
hex2code BALL.TGA G_ballIcon "const TUInt8" >BallIcon.h
hex2code FCTTEST.TGA G_funcTestIcon "const TUInt8" >FuncTestIcon.h
hex2code uparrow.TGA G_upArrowIcon "const TUInt8" >UpArrowIcon.h
hex2code downarrow.TGA G_downArrowIcon "const TUInt8" >DownArrowIcon.h
hex2code BOOMERANG.TGA G_funcTestLoopIcon "const TUInt8" >FuncTestLoopIcon.h
hex2code ReelSmall.TGA G_slideshowSmallIcon "const TUInt8" >SlideshowSmallIcon.h
hex2code pausebutton.tga G_pauseButtonIcon "const TUInt8" >PauseButtonIcon.h
hex2code stopbutton.tga G_stopButtonIcon "const TUInt8" >StopButtonIcon.h
hex2code playbutton.tga G_playButtonIcon "const TUInt8" >PlayButtonIcon.h
hex2code rewindbutton.tga G_rewindButtonIcon "const TUInt8" >RewindButtonIcon.h
hex2code forwardbutton.tga G_forwardButtonIcon "const TUInt8" >ForwardButtonIcon.h
hex2code AppFolder2.tga G_appFolderIcon "const TUInt8" >AppFolderIcon.h
hex2code QuestionIcon.tga G_questionIcon "const TUInt8" >QuestionIcon.h
hex2code TemperatureIcon2.tga G_temperatureIcon "const TUInt8" >TemperatureIcon.h
hex2code TimeDate.tga G_timeDateIcon "const TUInt8" >TimeDateIcon.h
hex2code RotatedBanner.tga G_rotatedBanner "const TUInt8" >RotatedBanner.h
hex2code DownExit.tga G_downExit "const TUInt8" >DownExit.h
hex2code YourAppWindow256.tga G_yourAppWindow "const TUInt8" >YourAppWindow.h
hex2code Contrast.tga G_contrastIcon "const TUInt8" >ContrastIcon.h
hex2code hourglass.tga G_hourglassIcon "const TUInt8" >HourglassIcon.h
hex2code VNC.TGA G_VNCIcon "const TUInt8" >VNCIcon.h
hex2code title.TGA G_titleScreen "const TUInt8" >TitleScreen.h
hex2code GainSpanSmall.TGA G_GainSpanLogo "const TUInt8" >GainSpanLogo.h
