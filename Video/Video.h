#pragma once
#include "locdef.h"

/************************************************************************/
/* Divide videos into images and combine images into videos				*/
/* Date: 07/28/2013	Version: 1.0										*/
/* Author: Shuang Feng	E-mail: safeng@umich.edu						*/
/************************************************************************/

namespace video
{
	// Divide video into images
	class VideoDiv
	{
	private:
		CvCapture * m_pCapture; // video capture from file or camera
		const IplImage * m_pCurImg; // current image pointer
		bool m_bUseCam; // use camera 

	public:
		explicit VideoDiv(const char * pfileNameStr);
		~VideoDiv(void);
		// save images & return actual number of saved images
		unsigned SaveImages(const char * pSaveDirPath); 

	private: // prohibit default copy constructor and assignment operator
		VideoDiv(const VideoDiv & vDivide){}
		VideoDiv & operator = (const VideoDiv & vDivide) {}
	};


	// Generate video from images or camera
	class VideoGen
	{
	private:
		CvVideoWriter * m_pVideoWriter;
		CvCapture * m_pCaptureCam; // video capturer for camera
		IplImage * m_pCurImg; // current image pointer loaded
		bool m_bCam; // camera source
		char m_imgPrefixStr[50]; // image prefix name
		char m_imgDirPathStr[150]; // image dir path
		char m_imgExtStr[10]; // image extension name

	public:
		VideoGen(const char * pImgDirPath, const char * pSaveVideoName, 
			const char * pImgPrefix = NULL, const char * pImgExt = NULL);
		~VideoGen(void);
		// save recorded video
		bool SaveVideo(unsigned numFrames = 0);

	private:
		VideoGen(const VideoGen & vGen) {}
		VideoGen & operator = (const VideoGen & vGen) {}
	};
};
