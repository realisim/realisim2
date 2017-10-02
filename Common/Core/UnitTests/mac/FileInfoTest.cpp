
#include <chrono>
#include <cstdio> // for file manipulation
#include <fstream> // for file writing.
#include "gtest/gtest.h"
#include <thread>
#include "Core/FileInfo.h"
#include "Core/Path.h"
//#include <Windows.h>

using namespace std;
using namespace Realisim;
using namespace Core;

namespace
{
	const string kStringContent = "The quick brown fox jumps over the lazy dog.";

	void removeFile(const std::string& iFilePath)
	{
		remove(iFilePath.c_str());
	}

	string readFile(const std::string& iFilePath)
	{
		string content;
		ifstream ifs;
		ifs.open(iFilePath, ios::in | ios::ate);
		if (!ifs.fail())
		{
			auto size = ifs.tellg();

			// create string at the right size
			content.resize(size, '\0');
			ifs.seekg(0);
			ifs.read(&content[0], size);
			ifs.close();
		}

		return content;
	}

	void writeFile(const std::string& iFilePath, const std::string& iContent)
	{
		ofstream ofs;
		ofs.open(iFilePath, ios::out | ios::app);
		if (!ofs.fail())
		{
			ofs.write(&iContent[0], iContent.size());
			ofs.close();
		}
	}
}

TEST(FileInfo, Constructor)
{
    string initializer1 = "C:/Windows/System32/mspaint.exe";
    string initializer2 = "C:/Windows/System32/different/mspaint.exe";
    
    // FileInfo();
    {
        FileInfo fi1;
        EXPECT_STREQ(fi1.getFilePath().c_str(), "");
    }
    
    // FileInfo(const string &filePath);
    {
        FileInfo fi2(initializer1);
        FileInfo fi2_1(initializer2);
        EXPECT_STREQ(fi2.getFilePath().c_str(), initializer1.c_str());
        EXPECT_STREQ(fi2_1.getFilePath().c_str(), initializer2.c_str());

        // FileInfo(const FileInfo &fileInfo) = default;
        FileInfo fi3(fi2);
        EXPECT_STREQ(fi3.getFilePath().c_str(), fi2.getFilePath().c_str());
        EXPECT_STRNE(fi3.getFilePath().c_str(), fi2_1.getFilePath().c_str());
    }
}

TEST(FileInfo, Functions)
{
	const string currentWorkingDirectory = Path::getCurrentWorkingDirectory();

	// we are temporarly setting the current working directory for testing the relative paths.
	// 
	const string temporaryCWD = "/tmp";
	Path::setCurrentWorkingDirectory(temporaryCWD);

    const string appPath = "/System/Library/Automator/Activate Fonts.action/Contents/Info.plist";
    const string appDirtyPath = "/System/../System\\/././///\\Library\\./Automator/Activate Fonts.action/././/Contents/Info.plist";
    
// NOT TOO SURE HOW TO HANDLE NETWORK PATH ON MAC!!!
//	const string networkPath = "\\\\simz800/Company/aXion Installers/v2";
//    const string networkUncPath = "\\\\\?\\UNC\\\\simz800/Company/aXion Installers/v2";
//	const string networkUncFilePath = "\\\\simz800\\Company\\aXion Installers\\Textures\\alt.rgb";
	const string folderPath = "/System/Library/";
	const string folderDirtyPath = "/System/\\/./.\\../System\\Library\\../Library\\";
	const string relativePath = "../../System/Library/Automator/Activate Fonts.action/Contents/Info.plist";
	const string nonExistingPath = "/patate\\oignon\\Confiture et jambon\\";
	const string nonExistingFilePath = "\\patate\\oignon\\Confiture et jambon\\a.b.c.d";

    FileInfo app(appPath);
    FileInfo appDirty(appDirtyPath);
//	FileInfo network(networkPath);
//    FileInfo unc(networkUncPath);
//	FileInfo uncFile(networkUncFilePath);
	FileInfo folder(folderPath);
	FileInfo folderDirty(folderDirtyPath);
	FileInfo relative(relativePath);
	FileInfo nonExisting(nonExistingPath);
	FileInfo nonExistingFile(nonExistingFilePath);
    
    // string absoluteFilePath() const;
    {
        EXPECT_STREQ(app.getAbsoluteFilePath().c_str(), "/System/Library/Automator/Activate Fonts.action/Contents/Info.plist");
        EXPECT_STREQ(appDirty.getAbsoluteFilePath().c_str(), "/System/../System/././Library/./Automator/Activate Fonts.action/././Contents/Info.plist");
//		EXPECT_STREQ(network.getAbsoluteFilePath().c_str(), "//simz800/Company/aXion Installers/v2");
//        EXPECT_STREQ(unc.getAbsoluteFilePath().c_str(), "//simz800/Company/aXion Installers/v2");
//		EXPECT_STREQ(uncFile.getAbsoluteFilePath().c_str(), "//simz800/Company/aXion Installers/Textures/alt.rgb");
		EXPECT_STREQ(folder.getAbsoluteFilePath().c_str(), "/System/Library/");
		EXPECT_STREQ(folderDirty.getAbsoluteFilePath().c_str(), "/System/././../System/Library/../Library/");
		EXPECT_STREQ(relative.getAbsoluteFilePath().c_str(), "/private/tmp/../../System/Library/Automator/Activate Fonts.action/Contents/Info.plist");
		EXPECT_STREQ(nonExisting.getAbsoluteFilePath().c_str(), "/patate/oignon/Confiture et jambon/");
		EXPECT_STREQ(nonExistingFile.getAbsoluteFilePath().c_str(), "/patate/oignon/Confiture et jambon/a.b.c.d");
    }
    
    // string absolutePath() const;
    {
		EXPECT_STREQ(app.getAbsolutePath().c_str(), "/System/Library/Automator/Activate Fonts.action/Contents");
		EXPECT_STREQ(appDirty.getAbsolutePath().c_str(), "/System/../System/././Library/./Automator/Activate Fonts.action/././Contents");
//		EXPECT_STREQ(network.getAbsolutePath().c_str(), "//simz800/Company/aXion Installers");
//		EXPECT_STREQ(unc.getAbsolutePath().c_str(), "//simz800/Company/aXion Installers");
//		EXPECT_STREQ(uncFile.getAbsolutePath().c_str(), "//simz800/Company/aXion Installers/Textures");
		EXPECT_STREQ(folder.getAbsolutePath().c_str(), "/System/Library");
		EXPECT_STREQ(folderDirty.getAbsolutePath().c_str(), "/System/././../System/Library/../Library");
		EXPECT_STREQ(relative.getAbsolutePath().c_str(), "/private/tmp/../../System/Library/Automator/Activate Fonts.action/Contents");
		EXPECT_STREQ(nonExisting.getAbsolutePath().c_str(), "/patate/oignon/Confiture et jambon");
		EXPECT_STREQ(nonExistingFile.getAbsolutePath().c_str(), "/patate/oignon/Confiture et jambon");
    }
    
    // string baseName() const;
    {
		EXPECT_STREQ(app.getBaseName().c_str(), "Info");
		EXPECT_STREQ(appDirty.getBaseName().c_str(), "Info");
//		EXPECT_STREQ(network.getBaseName().c_str(), "v2");
//		EXPECT_STREQ(unc.getBaseName().c_str(), "v2");
//		EXPECT_STREQ(uncFile.getBaseName().c_str(), "alt");
		EXPECT_STREQ(folder.getBaseName().c_str(), "");
		EXPECT_STREQ(folderDirty.getBaseName().c_str(), "");
		EXPECT_STREQ(relative.getBaseName().c_str(), "Info");
		EXPECT_STREQ(nonExisting.getBaseName().c_str(), "");
		EXPECT_STREQ(nonExistingFile.getBaseName().c_str(), "a");
    }

    // string canonicalFilePath() const;
    {
		EXPECT_STREQ(app.getCanonicalFilePath().c_str(), "/System/Library/Automator/Activate Fonts.action/Contents/Info.plist");
		EXPECT_STREQ(appDirty.getCanonicalFilePath().c_str(), "/System/Library/Automator/Activate Fonts.action/Contents/Info.plist");
//		EXPECT_STREQ(network.getCanonicalFilePath().c_str(), "//simz800/Company/aXion Installers/v2");
//		EXPECT_STREQ(unc.getCanonicalFilePath().c_str(), "//simz800/Company/aXion Installers/v2");
//		EXPECT_STREQ(uncFile.getCanonicalFilePath().c_str(), "//simz800/Company/aXion Installers/Textures/alt.rgb");
		EXPECT_STREQ(folder.getCanonicalFilePath().c_str(), "/System/Library/");
		EXPECT_STREQ(folderDirty.getCanonicalFilePath().c_str(), "/System/Library/");
		EXPECT_STREQ(relative.getCanonicalFilePath().c_str(), "/System/Library/Automator/Activate Fonts.action/Contents/Info.plist");
		EXPECT_STREQ(nonExisting.getCanonicalFilePath().c_str(), "/patate/oignon/Confiture et jambon/");
		EXPECT_STREQ(nonExistingFile.getCanonicalFilePath().c_str(), "/patate/oignon/Confiture et jambon/a.b.c.d");
    }
    
    // string canonicalPath() const;
    {
		EXPECT_STREQ(app.getCanonicalPath().c_str(), "/System/Library/Automator/Activate Fonts.action/Contents");
		EXPECT_STREQ(appDirty.getCanonicalPath().c_str(), "/System/Library/Automator/Activate Fonts.action/Contents");
//		EXPECT_STREQ(network.getCanonicalPath().c_str(), "//simz800/Company/aXion Installers");
//		EXPECT_STREQ(unc.getCanonicalPath().c_str(), "//simz800/Company/aXion Installers");
//		EXPECT_STREQ(uncFile.getCanonicalPath().c_str(), "//simz800/Company/aXion Installers/Textures");
		EXPECT_STREQ(folder.getCanonicalPath().c_str(), "/System/Library");
		EXPECT_STREQ(folderDirty.getCanonicalPath().c_str(), "/System/Library");
		EXPECT_STREQ(relative.getCanonicalPath().c_str(), "/System/Library/Automator/Activate Fonts.action/Contents");
		EXPECT_STREQ(nonExisting.getCanonicalPath().c_str(), "/patate/oignon/Confiture et jambon");
		EXPECT_STREQ(nonExistingFile.getCanonicalPath().c_str(), "/patate/oignon/Confiture et jambon");
    }
    
    // string completebaseName() const;
    {
		EXPECT_STREQ(app.getCompleteBaseName().c_str(), "Info");
		EXPECT_STREQ(appDirty.getCompleteBaseName().c_str(), "Info");
//		EXPECT_STREQ(network.getCompleteBaseName().c_str(), "");
//		EXPECT_STREQ(unc.getCompleteBaseName().c_str(), "");
//		EXPECT_STREQ(uncFile.getCompleteBaseName().c_str(), "alt");
		EXPECT_STREQ(folder.getCompleteBaseName().c_str(), "");
		EXPECT_STREQ(folderDirty.getCompleteBaseName().c_str(), "");
		EXPECT_STREQ(relative.getCompleteBaseName().c_str(), "Info");
		EXPECT_STREQ(nonExisting.getCompleteBaseName().c_str(), "");
		EXPECT_STREQ(nonExistingFile.getCompleteBaseName().c_str(), "a.b.c");
    }
    
    // string completeSuffix() const;
    {
		EXPECT_STREQ(app.getCompleteSuffix().c_str(), "plist");
		EXPECT_STREQ(appDirty.getCompleteSuffix().c_str(), "plist");
//		EXPECT_STREQ(network.getCompleteSuffix().c_str(), "");
//		EXPECT_STREQ(unc.getCompleteSuffix().c_str(), "");
//		EXPECT_STREQ(uncFile.getCompleteSuffix().c_str(), "rgb");
		EXPECT_STREQ(folder.getCompleteSuffix().c_str(), "");
		EXPECT_STREQ(folderDirty.getCompleteSuffix().c_str(), "");
		EXPECT_STREQ(relative.getCompleteSuffix().c_str(), "plist");
		EXPECT_STREQ(nonExisting.getCompleteSuffix().c_str(), "");
		EXPECT_STREQ(nonExistingFile.getCompleteSuffix().c_str(), "b.c.d");
    }
    
    // time_t creation() const;
	// time_t lastModificationTime() const;
	// time_t lastReadTime() const;
    {
		// sets tje cwd to create some file and play around with dates...
		//
		Path::setCurrentWorkingDirectory(currentWorkingDirectory);		

		const time_t now = time(0);

		//create a file
		FileInfo fi("./testFile.txt");
		removeFile(fi.getAbsoluteFilePath());
		writeFile(fi.getAbsoluteFilePath(), kStringContent);

		this_thread::sleep_for(chrono::seconds(1));
		fi.refresh(); // refresh since it is cached by default!
		// creation time is less or equal to now
        EXPECT_GE(fi.getCreationTime(), now); 
		const time_t creationTime = fi.getCreationTime();
		
		// sleep a bit modify file and check creation and lastModification data
		this_thread::sleep_for(chrono::seconds(2));
		writeFile(fi.getAbsoluteFilePath(), "\nMore and more content.");
		fi.refresh(); // refresh since it is cached by default!
        
        // THIS PART SEEMS TO BE QUITE DIFFERENT THAN ON WINDOWS...
        // it appears that openening a file and appending to it changed the
        // creation data instead of modification data...
        //
		EXPECT_GE(fi.getCreationTime(), creationTime);
        EXPECT_EQ(fi.getLastModificationTime(), fi.getCreationTime());
		const time_t lastModificationTime = fi.getLastModificationTime();
        
		// sleep a bit read the file and check dates..
		//
		this_thread::sleep_for(chrono::seconds(2));
		readFile(fi.getAbsoluteFilePath());
		fi.refresh(); // refresh since it is cached by default!
		EXPECT_EQ(fi.getCreationTime(), lastModificationTime);
		EXPECT_EQ(fi.getLastModificationTime(), lastModificationTime);
    }
    
    // bool exists() const;
    {
		EXPECT_TRUE(app.exists());
		EXPECT_TRUE(appDirty.exists());
//		EXPECT_TRUE(network.exists());
//		EXPECT_TRUE(unc.exists());
//		EXPECT_TRUE(uncFile.exists());
		EXPECT_TRUE(folder.exists());
		EXPECT_TRUE(folderDirty.exists());
		EXPECT_TRUE(relative.exists());
		EXPECT_FALSE(nonExisting.exists());
		EXPECT_FALSE(nonExistingFile.exists());
    }
    
    // string fileName() const;
    {
		EXPECT_STREQ(app.getFileName().c_str(), "Info.plist");
		EXPECT_STREQ(appDirty.getFileName().c_str(), "Info.plist");
//		EXPECT_STREQ(network.getFileName().c_str(), "v2");
//		EXPECT_STREQ(unc.getFileName().c_str(), "v2");
//		EXPECT_STREQ(uncFile.getFileName().c_str(), "alt.rgb");
		EXPECT_STREQ(folder.getFileName().c_str(), "");
		EXPECT_STREQ(folderDirty.getFileName().c_str(), "");
		EXPECT_STREQ(relative.getFileName().c_str(), "Info.plist");
		EXPECT_STREQ(nonExisting.getFileName().c_str(), "");
		EXPECT_STREQ(nonExistingFile.getFileName().c_str(), "a.b.c.d");
    }
    
    // string filePath() const;
    {
		EXPECT_STREQ(app.getFilePath().c_str(), "/System/Library/Automator/Activate Fonts.action/Contents/Info.plist");
		EXPECT_STREQ(appDirty.getFilePath().c_str(), "/System/../System/././Library/./Automator/Activate Fonts.action/././Contents/Info.plist");
//		EXPECT_STREQ(network.getFilePath().c_str(), "//simz800/Company/aXion Installers/v2");
//		EXPECT_STREQ(unc.getFilePath().c_str(), "//simz800/Company/aXion Installers/v2");
//		EXPECT_STREQ(uncFile.getFilePath().c_str(), "//simz800/Company/aXion Installers/Textures/alt.rgb");
		EXPECT_STREQ(folder.getFilePath().c_str(), "/System/Library/");
		EXPECT_STREQ(folderDirty.getFilePath().c_str(), "/System/././../System/Library/../Library/");
		EXPECT_STREQ(relative.getFilePath().c_str(), "../../System/Library/Automator/Activate Fonts.action/Contents/Info.plist");
		EXPECT_STREQ(nonExisting.getFilePath().c_str(), "/patate/oignon/Confiture et jambon/");
		EXPECT_STREQ(nonExistingFile.getFilePath().c_str(), "/patate/oignon/Confiture et jambon/a.b.c.d");

    }
    
    // bool isAbsolute() const;
    {
		EXPECT_TRUE(app.isAbsolute());
		EXPECT_TRUE(appDirty.isAbsolute());
//		EXPECT_TRUE(network.isAbsolute());
//		EXPECT_TRUE(unc.isAbsolute());
//		EXPECT_TRUE(uncFile.isAbsolute());
		EXPECT_TRUE(folder.isAbsolute());
		EXPECT_TRUE(folderDirty.isAbsolute());
		EXPECT_FALSE(relative.isAbsolute());
		EXPECT_TRUE(nonExisting.isAbsolute());
		EXPECT_TRUE(nonExistingFile.isAbsolute());
    }
    
    // bool cached() const;
    {
		EXPECT_TRUE(app.isCached());
		EXPECT_TRUE(appDirty.isCached());
//		EXPECT_TRUE(network.isCached());
//		EXPECT_TRUE(unc.isCached());
//		EXPECT_TRUE(uncFile.isCached());
		EXPECT_TRUE(folder.isCached());
		EXPECT_TRUE(folderDirty.isCached());

		relative.setCached(false);
		EXPECT_FALSE(relative.isCached());
    }    

    // bool isDir() const;
    {
		EXPECT_FALSE(app.isDir());
		EXPECT_FALSE(appDirty.isDir());
//		EXPECT_TRUE(network.isDir());
//		EXPECT_TRUE(unc.isDir());
//		EXPECT_FALSE(uncFile.isDir());
		EXPECT_TRUE(folder.isDir());
		EXPECT_TRUE(folderDirty.isDir());
		EXPECT_FALSE(relative.isDir());
		EXPECT_FALSE(nonExisting.isDir());
		EXPECT_FALSE(nonExistingFile.isDir());
    }
    
    // bool isFile() const;
    {
		EXPECT_TRUE(app.isFile());
		EXPECT_TRUE(appDirty.isFile());
//		EXPECT_FALSE(network.isFile());
//		EXPECT_FALSE(unc.isFile());
//		EXPECT_TRUE(uncFile.isFile());
		EXPECT_FALSE(folder.isFile());
		EXPECT_FALSE(folderDirty.isFile());
		EXPECT_TRUE(relative.isFile());
		EXPECT_FALSE(nonExisting.isFile());
		EXPECT_FALSE(nonExistingFile.isFile());
    }
    
    // bool isRelative() const;
    {
		EXPECT_FALSE(app.isRelative());
		EXPECT_FALSE(appDirty.isRelative());
//		EXPECT_FALSE(network.isRelative());
//		EXPECT_FALSE(unc.isRelative());
//		EXPECT_FALSE(uncFile.isRelative());
		EXPECT_FALSE(folder.isRelative());
		EXPECT_FALSE(folderDirty.isRelative());
		EXPECT_TRUE(relative.isRelative());
		EXPECT_FALSE(nonExisting.isRelative());
		EXPECT_FALSE(nonExistingFile.isRelative());
    }
    
    // bool isRoot() const;
    {
		FileInfo root0("/");
		FileInfo root1("c:\\");
		FileInfo root2("C:/");
		FileInfo root3("\\\\Server");
		FileInfo root4("\\\\Server\\");
		EXPECT_FALSE(app.isRoot());
		EXPECT_FALSE(appDirty.isRoot());
		EXPECT_TRUE(root0.isRoot());
		EXPECT_FALSE(root1.isRoot());
		EXPECT_FALSE(root2.isRoot());
		EXPECT_TRUE(root3.isRoot());
		EXPECT_TRUE(root4.isRoot());

    }
    
    // bool isSymlink() const;
    {
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// creating a simlink requires admin privilege... so this test
		// must run in admin to succeed.
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//
//		FileInfo symLink(".\\symlinkToMspaint.exe");
//		FileInfo target("C:\\Windows\\System32\\mspaint.exe");
//
//		CreateSymbolicLink(symLink.getCanonicalFilePath().c_str(), target.getAbsoluteFilePath().c_str(), 0x0);
//		symLink.refresh();
//
//        EXPECT_FALSE(paint.isSymlink());
//        EXPECT_TRUE(symLink.isSymlink());
    }
    
    // time_t lastModificationTime() const;
    {
		//tested higher in creationTime   
    }
    
    // bool makeAbsolute();
    {
		FileInfo aRelativePath("..\\something");
        EXPECT_FALSE(app.makeAbsolute());
        EXPECT_TRUE(aRelativePath.makeAbsolute());
    }
    

    // string path() const;
    {
		EXPECT_STREQ(app.getPath().c_str(), "/System/Library/Automator/Activate Fonts.action/Contents");
		EXPECT_STREQ(appDirty.getPath().c_str(), "/System/../System/././Library/./Automator/Activate Fonts.action/././Contents");
//		EXPECT_STREQ(network.getPath().c_str(), "\\\\simz800\\Company\\aXion Installers");
//		EXPECT_STREQ(unc.getPath().c_str(), "\\\\simz800\\Company\\aXion Installers");
//		EXPECT_STREQ(uncFile.getPath().c_str(), "\\\\simz800\\Company\\aXion Installers\\Textures");
		EXPECT_STREQ(folder.getPath().c_str(), "/System/Library");
		EXPECT_STREQ(folderDirty.getPath().c_str(), "/System/././../System/Library/../Library");
		EXPECT_STREQ(relative.getPath().c_str(), "../../System/Library/Automator/Activate Fonts.action/Contents");
		EXPECT_STREQ(nonExisting.getPath().c_str(), "/patate/oignon/Confiture et jambon");
		EXPECT_STREQ(nonExistingFile.getPath().c_str(), "/patate/oignon/Confiture et jambon");
    }
    
    // void refresh();
    {
        //tested higher in creationTime()
    }
    
    // void setCached(bool cached);
    {
        
    }
    
    // void setFile(const string &filePath);
    {
		FileInfo fi;
		fi.setFile("/test.txt");
		EXPECT_STREQ(fi.getFilePath().c_str(), "/test.txt");
    }
    
    // void setFile(const string &dirpath, const string &filename);
    {
		FileInfo fi;
		fi.setFile("/tmp", "test.txt");
		EXPECT_STREQ(fi.getFilePath().c_str(), "/tmp/test.txt");
    }
    
    // unsigned long long size() const;
    {
        EXPECT_EQ(app.getSize(), 2927);
    }
    
    // string suffix() const;
    {
		EXPECT_STREQ(app.getSuffix().c_str(), "plist");
		EXPECT_STREQ(appDirty.getSuffix().c_str(), "plist");
//		EXPECT_STREQ(network.getSuffix().c_str(), "");
//		EXPECT_STREQ(unc.getSuffix().c_str(), "");
//		EXPECT_STREQ(uncFile.getSuffix().c_str(), "rgb");
		EXPECT_STREQ(folder.getSuffix().c_str(), "");
		EXPECT_STREQ(folderDirty.getSuffix().c_str(), "");
		EXPECT_STREQ(relative.getSuffix().c_str(), "plist");
		EXPECT_STREQ(nonExisting.getSuffix().c_str(), "");
		EXPECT_STREQ(nonExistingFile.getSuffix().c_str(), "d");
    }
    
    // string symlinkTarget() const;
    {
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// creating a simlink requires admin privilege... so this test
		// must run in admin to succeed.
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//
//		FileInfo symLink(".\\symlinkToMspaint.exe");
//		FileInfo target("C:\\Windows\\System32\\mspaint.exe");
//
//		CreateSymbolicLink(symLink.getCanonicalFilePath().c_str(), target.getAbsoluteFilePath().c_str(), 0x0);
//		symLink.refresh();
//
//        EXPECT_STREQ(paint.getSymlinkTarget().c_str(), "C:\\Windows\\System32\\mspaint.exe");
//        EXPECT_STREQ(symLink.getSymlinkTarget().c_str(), "C:\\Windows\\System32\\mspaint.exe");
    }
    
}
