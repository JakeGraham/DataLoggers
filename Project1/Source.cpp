#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<conio.h>
//#include<sstream>



int main()
 {
	std::string sourcefolder;
	std::string copyfolder;
	std::string OPfolder;

	std::cout << "Enter the filepath to a directory containing ONLY the .txt data files from a logger" << std::endl;
	std::cin >> sourcefolder;
	std::cout << " " << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << " " << std::endl;
	std::cout << "." << std::endl;
	std::cout << ".." << std::endl;
	std::cout << "..." << std::endl;
	std::cout << "...." << std::endl;
	std::cout << "..." << std::endl;
	std::cout << ".." << std::endl;
	std::cout << "." << std::endl;
	std::cout << " " << std::endl;
	std::cout << "	Processing data, this will take a few seconds.... \n";
	std::cout << " " << std::endl;
	std::cout << "." << std::endl;
	std::cout << ".." << std::endl;
	std::cout << "..." << std::endl;
	std::cout << "...." << std::endl;
	std::cout << "..." << std::endl;
	std::cout << ".." << std::endl;
	std::cout << "." << std::endl;

	// creates a temporary directory to work in
	copyfolder = sourcefolder + "\\MERGED_TEMP12151";
	std::string temps = "if not exist " + copyfolder + " mkdir " + copyfolder;

	char cmd[150];
	std::strcpy(cmd, temps.c_str());
	system(cmd);
	
	// copy data files to copy folder
	std::string src2cpy = "copy " + sourcefolder + "\\*.txt " + copyfolder + " >nul";
	char temp[150];
	std::strcpy(temp, src2cpy.c_str());
	system(temp);
	
	// data munge to combine all logger files into one and put column headings
	std::ofstream batch;
	batch.open("mybatchfile131.bat", std::ios::out);
	batch << "@echo off" << std::endl;
	batch << "for %%t in (" << copyfolder << "\\*.txt) do (" << std::endl;
	batch << "more +3 \"%%~ft\" > \"%%~ft.tmp\"" << std::endl;
	batch << "move /y \"%%~ft.tmp\" \"%%~ft\" >nul)" << std::endl;
	batch << "copy " << copyfolder << "\\*.txt " << copyfolder << "\\OGFormatCombined.arb >nul" << std::endl;
	batch << "del " << copyfolder << "\\*.txt" << std::endl;
	batch << "ren  " << copyfolder << "\\OGFormatCombined.arb  OGFormatCombined.txt" << std::endl;
	batch << "echo time,bat,temp,DO,Q > " << copyfolder << "\\OGFormatCombinedtemp.txt" << std::endl;
	batch << "type " << copyfolder << "\\OGFormatCombined.txt" << " >> " << copyfolder << "\\OGFormatCombinedtemp.txt" << std::endl;
	batch << "type " << copyfolder << "\\OGFormatCombinedtemp.txt" << " > " << copyfolder << "\\OGFormatCombined.txt" << std::endl;
	batch << "del " << copyfolder << "\\OGFormatCombinedtemp.txt";
	batch.close();
	system("mybatchfile131.bat");
	system("del mybatchfile131.bat");

	// create a directory to output to
	OPfolder = sourcefolder + "\\LoggerOutput";
	std::string temps2 = "if not exist " + OPfolder + " mkdir " + OPfolder;
	char mkOP[150];
	std::strcpy(mkOP, temps2.c_str());
	system(mkOP);

	// copy contents of copy directory to output directory
	std::string cpy2OP = "copy " + copyfolder + " " + OPfolder + " >nul";
	char mvOP[150];
	std::strcpy(mvOP, cpy2OP.c_str());
	system(mvOP);

	// delete copy directory
	std::string delcpy = "rd " + copyfolder + " /q /s";
	char delc[150];
	std::strcpy(delc, delcpy.c_str());
	system(delc);
	
	// creat a string to the, to be produced, R script
	std::string P2Rscript = OPfolder + "\\LoggerScript.R";
	char RSS[150];
	std::strcpy(RSS, P2Rscript.c_str());
	
	// parse the path into the format R uses
	std::string s = OPfolder;
	std::string delimiter = "\\";
	std::vector<std::string> Raddress;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		Raddress.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	std::string RAD = Raddress[0];
	for (int i = 1; i < Raddress.size(); i++) {
		RAD = RAD + "\\\\" + Raddress[i];
	}

	// create the R script to be used/produced
	std::ofstream Rscript;
	Rscript.open(RSS, std::ios::out);
	Rscript << "data<-read.csv(\"" + RAD + "\\\\LoggerOutput\\\\OGFormatCombined.txt\")\n";
	Rscript << "\n";
	Rscript << "# years since 1969.....the way its formatted.....\n";
	Rscript << "ys69<-floor(max(data$time / 86400) / 365.25)\n";
	Rscript << "\n";
	Rscript << "# days since 1969..exluding the current year\n";
	Rscript << "ds69<-0\n";
	Rscript << "for (i in 1 : ys69) {\n";
	Rscript << "if ((i + 1) %%4 == 0) {\n";
	Rscript << "ds69 <-ds69 + 366\n";
	Rscript <<"}\n";
	Rscript << "else {\n";
	Rscript << "ds69 <-ds69 + 365\n";
	Rscript << "}\n";
	Rscript << "if (i == ys69) {\n";
	Rscript << "currentyear<-1969 + i + 1\n";
	Rscript << "}\n";
	Rscript << "}\n";
	Rscript << "\n";
	Rscript << "#  Julian Day w / decimal\n";
	Rscript << "data$day2<-(data$time / 86400) - (ys69*365.25)";
	Rscript << "\n"; 
	Rscript << "#  Julian Day Whole\n";
	Rscript << "data$day<-floor(data$day2)\n";
	Rscript << "\n";
	Rscript << "# Year\n";
	Rscript << "Year<-rep(NA, length(data$day))\n";
	Rscript << "for (i in 1 : length(data$day)) {\n";
	Rscript << "if (data$day[i] >= 0) {\n";
	Rscript << "Year[i] = currentyear\n";
	Rscript << "}\n";
	Rscript << "else {\n";
	Rscript << "Year[i] = currentyear - 1\n";
	Rscript << "}\n";
	Rscript << "}\n";
	Rscript << "data$year<-Year\n";
	Rscript << "# Hour\n";
	Rscript << "data$hour<-rep(NA, length(data$day))\n";
	Rscript << "data$hour2<-rep(NA, length(data$day))\n";
	Rscript << "for (i in 1 : length(data$hour)) {\n";
	Rscript << "data$hour[i]<-floor((data$day2[i] - floor(data$day2[i])) / (1 / 24))\n";
	Rscript << "data$hour2[i]<-(data$day2[i] - floor(data$day2[i])) / (1 / 24)\n";
	Rscript << "}\n";
	Rscript <<"\n";
	Rscript << "# Min\n";
	Rscript << "data$min<-rep(NA, length(data$day))\n";
	Rscript << "for (i in 1 : length(data$min)) {\n";
	Rscript << "data$min[i]<-floor((data$hour2[i] - floor(data$hour2[i])) / (1 / 60))\n";
	Rscript <<"}\n";
	Rscript << "\n";
	Rscript << "# Fully cleaned dataset\n";
	Rscript << "df0<-as.data.frame(cbind(data$year, data$day, data$hour, data$min, data$DO, data$temp))\n";
	Rscript << "colnames(df0)<-c(\"Year\", \"JulianDay\", \"Hour\", \"Min\", \"DO (mg/L)\", \"Temp (C)\")\n";
	Rscript << "\n";
	Rscript << "# Daily mean temps and DOs\n";
	Rscript << "MDO<-as.matrix(tapply(data$DO, as.factor(data$day), mean))\n";
	Rscript << "MDT<-as.matrix(tapply(data$temp, as.factor(data$day), mean))\n";
	Rscript << "Ds<-as.numeric(row.names(as.matrix(tapply(data$DO, as.factor(data$day), mean))))\n";
	Rscript << "# Year\n";
	Rscript << "Year2<-rep(NA, length(Ds))\n";
	Rscript << "for (i in 1 : length(Ds)) {\n";
	Rscript << "if (Ds[i] >= 0) {\n";
	Rscript << "Year2[i] = currentyear\n";
	Rscript <<"}\n";
	Rscript << "else {\n";
	Rscript << "Year2[i] = currentyear - 1\n";
	Rscript <<"}\n";
	Rscript << "}\n";
	Rscript << "df1<-as.data.frame(cbind(Year2, Ds, MDO, MDT))\n";
	Rscript << "colnames(df1)<-c(\"Year\", \"JulianDay\", \"MeanDO (mg/L)\", \"MeanTemp (C)\")\n";
	Rscript << "\n";
	Rscript << "# Hourly mean temps and DOs\n";
	Rscript << "MHDO<-vector(mode = \"numeric\", )\n";
	Rscript << "MHT<-vector(mode = \"numeric\", )\n";
	Rscript << "MHD<-vector(mode = \"numeric\", )\n";
	Rscript << "MHY<-vector(mode = \"numeric\", )\n";
	Rscript << "MHH<-vector(mode = \"numeric\", )\n";
	Rscript << "for (i in min(data$day) : max(data$day)) {\n";
	Rscript << "for (j in min(data$hour) : max(data$hour)) {\n";
	Rscript << "if (length(data$DO[data$day == i & data$hour == j])>1) {\n";
	Rscript << "MHDO<-append(MHDO, mean(data$DO[data$day == i & data$hour == j]))\n";
	Rscript << "MHT<-append(MHT, mean(data$temp[data$day == i & data$hour == j]))\n";
	Rscript << "MHY<-append(MHY, data$year[data$day == i & data$hour == j][1])\n";
	Rscript << "MHD<-append(MHD, i)\n";
	Rscript << "MHH<-append(MHH, j)\n";
	Rscript <<"}\n";
	Rscript << "else if (length(data$DO[data$day == i & data$hour == j]) == 1) {\n";
	Rscript << "MHDO<-append(MHDO, data$DO[data$day == i & data$hour == j])\n";
	Rscript << "MHT<-append(MHT, data$temp[data$day == i & data$hour == j])\n";
	Rscript << "MHY<-append(MHY, data$year[data$day == i & data$hour == j][1])\n";
	Rscript << "MHD<-append(MHD, i)\n";
	Rscript << "MHH<-append(MHH, j)\n";
	Rscript <<"}\n";
	Rscript <<"}\n";
	Rscript <<"}\n";
	Rscript << "\n";
	Rscript << "\n";
	Rscript << "\n"; 
	Rscript << "df2<-as.data.frame(cbind(MHY, MHD, MHH, MHDO, MHT))\n";
	Rscript << "colnames(df2)<-c(\"Year\", \"JulianDay\", \"Hour\", \"MeanDO (mg/L)\", \"MeanTemp (C)\")\n";
	Rscript << "\n";
	Rscript << "#################\n";
	Rscript << "#################	Output figures\n";
	Rscript << "#################\n";
	Rscript << "setwd(\"" << RAD << "\\\\LoggerOutput\")\n";
	Rscript << "\n";
	Rscript << "# DO\n";
	Rscript << "png(filename = \"DO.png\", width = 1080, height = 720)\n";
	Rscript << "par(mai = c(1, 1, .1, .25))\n";
	Rscript << "plot(data$DO~data$day2, ylab = \"DO (mg/L)\", xlab = \"Julian Day\", ylim = c(0, max(data$DO)*1.1), bty = \"l\", cex.lab = 1.5)\n";
	Rscript << " graphics.off()\n";
	Rscript << "\n";
	Rscript << "# Daily Mean DO\n";
	Rscript << "png(filename = \"MeanDailyDO.png\", width = 1080, height = 720)\n";
	Rscript << "par(mai = c(1, 1, .1, .25))\n";
	Rscript << "plot(df1$MeanDO~df1$JulianDay, ylab = \"Daily Mean DO (mg/L)\", xlab = \"Julian Day\", ylim = c(0, max(df1$MeanDO)*1.1), bty = \"l\", cex.lab = 1.5, pch = 16)\n";
	Rscript << "graphics.off()\n";
	Rscript << "\n";
	Rscript << "# Hourly Mean DO\n";
	Rscript << "png(filename = \"MeanHourlyDO.png\", width = 1080, height = 720)\n";
	Rscript << "par(mai = c(1, 1, .1, .25))\n";
	Rscript << "plot(df2$MeanDO~df2$Hour, ylab = \"Hourly Mean DO (mg/L)\", xlab = \"Hour\", ylim = c(0, max(df2$MeanDO)*1.1), bty = \"l\", cex.lab = 1.5, pch = 16, col = df2$JulianDay, cex = 110 / df2$JulianDay)\n";
	Rscript << "legend(\"top\", \"Colors Represent Days\", bty = \"n\")\n";
	Rscript << "graphics.off()\n";
	Rscript << "\n";
	Rscript << "# Temp\n";
	Rscript << "png(filename = \"Temp.png\", width = 1080, height = 720)\n";
	Rscript << "par(mai = c(1, 1, .1, .25))\n";
	Rscript << "plot(data$temp~data$day2, ylab = \"Temp (C)\", xlab = \"Julian Day\", ylim = c(0, max(data$temp)*1.1), pch = 16, bty = \"l\", cex.lab = 1.5)\n";
	Rscript << "graphics.off()\n";
	Rscript << "\n";
	Rscript << "# Daily Mean Temp\n";
	Rscript << "png(filename = \"MeanDailyTemp.png\", width = 1080, height = 720)\n";
	Rscript << "par(mai = c(1, 1, .1, .25))\n";
	Rscript << "plot(df1$MeanTemp~df1$JulianDay, ylab = \"Daily Mean Temp (C)\", xlab = \"Julian Day\", ylim = c(0, max(df1$MeanTemp)*1.1), bty = \"l\", cex.lab = 1.5, pch = 16)\n";
	Rscript << "graphics.off()\n";
	Rscript << "\n";
	Rscript << "# Hourly Mean Temp\n";
	Rscript << "png(filename = \"MeanHourlyTemp.png\", width = 1080, height = 720)\n";
	Rscript << "par(mai = c(1, 1, .1, .25))\n";
	Rscript << "plot(df2$MeanTemp~df2$Hour, ylab = \"Hourly Mean Temp (C)\", xlab = \"Hour\", ylim = c(0, max(df2$MeanTemp)*1.1), bty = \"l\", cex.lab = 1.5, pch = 16, col = df2$JulianDay, cex = 110 / df2$JulianDay)\n";
	Rscript << "legend(\"top\", \"Colors Represent Days\", bty = \"n\")\n";
	Rscript << "graphics.off()\n";
	Rscript << "\n";
	Rscript << "# DO and Temp\n";
	Rscript << "png(filename = \"DOTemp.png\", width = 1080, height = 720)\n";
	Rscript << "par(mar = c(5, 4, 4, 4) + 0.3)\n";
	Rscript << "plot(data$DO~data$day2, ylab = \"DO (mg/L)\", xlab = \"Julian Day\", ylim = c(0, max(df1$MeanDO)*1.1), bty = \"u\", pch = 16, cex = .25, cex.lab = 1.5)\n";
	Rscript << "par(new = TRUE)\n";
	Rscript << "plot(data$temp~data$day2, axes = FALSE, bty = \"n\", xlab = \"\", ylab = \"\", col = \"red\", pch = 16, cex = .25, cex.lab = 1.5)\n";
	Rscript << "axis(side = 4, at = pretty(range(data$temp)), col.axis = \"red\")\n";
	Rscript << "mtext(\"Temp (C)\", side = 4, line = 3, col = \"red\", cex = 1.5)\n";
	Rscript << "legend(\"top\", c(\"Temp\", \"DO\"), col = c(\"red\", \"black\"), pch = 16, bty = \"n\")\n";
	Rscript << "graphics.off()\n";
	Rscript << "\n";
	Rscript << "# Mean Daily DO and Temp\n";
	Rscript << "png(filename = \"DailyMeanDOTemp.png\", width = 1080, height = 720)\n";
	Rscript << "par(mar = c(5, 4, 4, 4) + 0.3)\n";
	Rscript << "plot(df1$MeanDO~df1$JulianDay, ylab = \"Daily Mean DO (mg/L)\", xlab = \"Julian Day\", ylim = c(0, max(df1$MeanDO)*1.1), bty = \"l\", cex.lab = 1.5, pch = 16)\n";
	Rscript << "par(new = TRUE)\n";
	Rscript << "plot(df1$MeanTemp~df1$JulianDay, axes = FALSE, bty = \"n\", xlab = \"\", ylab = \"\", col = \"red\", pch = 16, cex.lab = 1.5)\n";
	Rscript << "axis(side = 4, at = pretty(range(df1$MeanTemp)), col.axis = \"red\")\n";
	Rscript << "mtext(\"Daily Mean Temp (C)\", side = 4, line = 3, col = \"red\", cex = 1.5)\n";
	Rscript << "legend(\"top\", c(\"Temp\", \"DO\"), col = c(\"red\", \"black\"), pch = 16, bty = \"n\")\n";
	Rscript << "\n";
	Rscript << "# csv with fully cleaned dataset\n";
	Rscript << "write.csv(df0, \"CleanedLoggerData.csv\", row.names = FALSE)\n";
	Rscript << "\n";
	Rscript << "# output csv with daily means\n";
	Rscript << "write.csv(df1, \"DailyMeans.csv\", row.names = FALSE)\n";
	Rscript << "\n";
	Rscript << "# csv with hourly means\n";
	Rscript << "write.csv(df2,\"HourlyMeans.csv\",row.names=FALSE)\n";
	Rscript.close();

	// find the path to Rscript.exe and run the above script
	std::ofstream Rbatch;
	Rbatch.open("mybatchfile132.bat", std::ios::out);
	Rbatch << "@echo off" << std::endl;
	Rbatch << "for /f \"delims=\" %%F in ('WHERE /R \"c:\\Program Files\" Rscript') do set var=\"%%F\"" << std::endl;
	Rbatch << "%var% " << OPfolder << "\\LoggerScript.R" << std::endl;
	Rbatch.close();
	system("mybatchfile132.bat");
	system("del mybatchfile132.bat");


	std::cout << " " << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << " " << std::endl;
	std::cout << "." << std::endl;
	std::cout << ".." << std::endl;
	std::cout << "..." << std::endl;
	std::cout << "...." << std::endl;
	std::cout << "..." << std::endl;
	std::cout << ".." << std::endl;
	std::cout << "." << std::endl;
	std::cout << " " << std::endl;
	std::cout << "	Your data files, figures, and script can be found in " << OPfolder << std::endl;
	std::cout << " " << std::endl;
	std::cout << "." << std::endl;
	std::cout << ".." << std::endl;
	std::cout << "..." << std::endl;
	std::cout << "....						Press any key to exit...\n";
	std::cout << "..." << std::endl;
	std::cout << ".." << std::endl;
	std::cout << "." << std::endl;
	std::cout << " " << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	getch();

	// open DO.temp up on exiting the program
	std::string DOTEMP = OPfolder + "\\DOTemp.png";
	char temp3[150];
	std::strcpy(temp3, DOTEMP.c_str());
	system(temp3);

	exit(0);
	return 0;
}