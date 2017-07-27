**************************************************************************************************

			***************	  A Few Things to Mention...	******************

**************************************************************************************************	

~~~~~~~ !!!! This program will only work on machines that run windows (i.e., no linux or Mac) !!!
~~~~~~~ If the file named "CombineLoggerFiles" does not have an ".exe" extension right click and rename it to add the .exe., to create "CombineLoggerFiles.exe"
~~~~~~~ The file does not have the .exe because gmail does not allow sending of executables... 
~~~~~~~ To get the full functionality you need to have Program R installed on your machine, as all the secondary processing is done through a wrapper for R
~~~~~~~ Program R is free and available here  https://cran.r-project.org/bin/windows/base/
~~~~~~~ I recommend installing if you do not already have it..

**************************************************************************************************

			**************		Instructions		**************

**************************************************************************************************			

~~~~~~~ To use the program first have a directory (folder) containing only .txt files from a single data logger
~~~~~~~ Double click "CombineLoggerFiles.exe" you will be prompted to provide a filepath to the directory containing logger files
~~~~~~~ Type the file path of the directory containing your text files 
~~~~~~~ Then press enter
~~~~~~~	For example 	~	C:\Users\jake_\Desktop\dataloggers	~
~~~~~~~ If you don't want to type the address you can find the path by right clicking on one of the .txt data files and hit "properties"
~~~~~~~ Then copy (right click > copy, or highlight + ctrl+c) and paste (right click, or ctrl+v) "location" into the program's command prompt and press enter
~~~~~~~ It will take the program a few seconds (depending on how many files there are) to processes the data
~~~~~~~ There will now be a new directory named "LoggerOutput" within the directory you provided, which contains the following outputs

**************************************************************************************************

			**************		Outputs		**************

**************************************************************************************************

~~ 1: OGFormatCombined.txt
	A .txt csv containing all of the logger files combined in the original format
		column names:
			"time" = seconds since 1969... which is the format they provide it in... I have no idea why...
			"bat" = battery life
			"temp" = temperature in C
			"DO" = DO in mg/L
			"Q" = the quality measure... not sure exactly how this is measured

~~ 2: LoggerScript.R 
	The R script used to process OGFormatCombined.txt and produce all subsequent outputs.. (the program wraps R to run this script)


~~~~~~~ !!!! If you do not have R installed OGFormatCombined.txt & LoggerScript.R (which is then pretty useless..) are the only outputs you will get !!!


******* If R is installed you will also get the following *******

~~ 3: CleanedLoggerData.csv
	A .csv containing the data from all original .txt files put into a much more user friendly format
		Column names:
			"Year" = ... the year
			"JulianDay" = Julian Day acounting for leap years
			"Hour" = the hour of day using the 24 hour clock
			"Min" = ... the minute
			"DO (mg/L)" = Dissolved oxygen in mg/L...
			"Temp (C)" = Temperature in celcius

~~ 4: DailyMeans.csv
	A .csv containing mean daily temperatures & DOs
		Column names:
			"Year" = ... it's the year
			"JulianDay" = Julian Day acounting for leap years
			"MeanDO (mg/L)" = Mean dissolved oxygen in mg/L for each day it was deployed
			"MeanTemp (C)" = Mean temperature in celcius for each day it was deployed

~~ 5: HourlyMeans.csv
	A .csv containing mean hourly temperatures & DOs
		Column names:
			"Year" = ... it's the year
			"JulianDay" = Julian Day acounting for leap years
			"Hour" = the hour of day using the 24 hour clock
			"MeanDO (mg/L)" = Mean dissolved oxygen in mg/L for each hour it was deployed
			"MeanTemp (C)" = Mean temperature in celcius for each hour it was deployed


~~ 6: DO.png
	A figure of DO (mg/L) as a function of Julian day for each data point


~~ 7: Temp.png
	A figure of temperature (C) as a function of Julian day for each data point


~~ 8: DOTemp.png
	A figure of DO (mg/L) & temperature (C) as a function of Julian day for each data point, with DO in black on the primary axis
	and temperature in red on the secondary axis


~~ 9: DailyMeanDO.png
	A figure of mean daily DO (mg/L) as a function of Julian day for each day


~~ 10: DailyMeanTemp.png
	A figure of mean temperature (C) as a function of Julian day for each day


~~ 11: DailyMeanDOTemp.png
	A figure of daily mean DO (mg/L) & temperature (C) as a function of Julian day for each day, with DO in black on the primary axis
	and temperature in red on the secondary axis

~~ 12: MeanHourlyDO.png
	A figure of mean hourly DO (mg/L) as a function of hour (24 hour clock), different colors represent different days.. there are likely more days than colors
	so colors will be reused


~~ 13: MeanHourlyTemp.png
	A figure of mean hourly temperature (C) as a function of (24 hour clock), different colors represent different days.. there are likely more days than colors
	so colors will be reused

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~   When the program finishes pressing any key will exit and open DOTemp.png	~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**************************************************************************************************

***************   The c++ source code (DLOG_source.cpp) should be provided 	   ***************
***************   with the executable if anybody wishes to make modifications	   ***************

***************   		Hopfully this is helpful...			   ***************
***************    If you have questions my email is jakegraham@u.boisestate.edu   ***************

**************************************************************************************************












	
