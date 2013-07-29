#include "Video.h"

namespace video
{
	VideoDiv::VideoDiv(const char * pfileNameStr)
		: m_pCapture(NULL), m_pCurImg(NULL)
	{
		if (NULL == pfileNameStr) // read from camera
		{
			m_pCapture = cvCreateCameraCapture(0);
		}else
		{
			m_pCapture = cvCreateFileCapture(pfileNameStr);
		}

		if (!m_pCapture)
		{
			std::cerr << "Create video capture error." << std::endl;
			return;
		}
	}

	VideoDiv::~VideoDiv(void)
	{
		if (m_pCapture)
		{
			cvReleaseCapture(&m_pCapture);
		}
	}

	unsigned VideoDiv::SaveImages(const char * pSaveDirPath)
	{
		if (!pSaveDirPath || !m_pCapture)
		{
			return 0;
		}

		if (!_access(pSaveDirPath,0)) // dir path does not exist
		{
			std::cerr << "Saving path does not exist" << std::endl;
			return 0;
		}
		
		const char * pWinNameStr = "Video Divider";
		cvNamedWindow(pWinNameStr);
		char nameBufArray[200];
		int count = 0;
		// Query frame
		while (m_pCurImg=cvQueryFrame(m_pCapture))
		{
			sprintf_s(nameBufArray,"%s\\img%d.jpg",pSaveDirPath,count);
			cvShowImage(pWinNameStr,m_pCurImg); // show image
			cvSaveImage(nameBufArray,m_pCurImg); // save image
			++count;
			cvWaitKey(10);
		}

		cvDestroyWindow(pWinNameStr);
		return count+1;
	}

	VideoGen::VideoGen(const char * pImgDirPath, const char * pSaveVideoName,
		const char * pImgPrefix, const char * pImgExt)
		: m_pVideoWriter(NULL), m_pCaptureCam(NULL), m_pCurImg(NULL)
	{
		if (NULL == pImgDirPath) // record video from camera
		{
			m_bCam = true;
			m_pCaptureCam = cvCreateCameraCapture(0);
			if (!m_pCaptureCam)
			{
				std::cerr << "Open camera error." << std::endl;
				return;
			}
			// get the first frame
			m_pCurImg = cvQueryFrame(m_pCaptureCam);

		}else
		{
			m_bCam = false;
			if (_access(pImgDirPath,0))
			{
				std::cerr << "Path does not exist." <<std::endl;
				return;
			}
			strcpy_s(m_imgDirPathStr,pImgDirPath);
			strcpy_s(m_imgPrefixStr,pImgPrefix);
			strcpy_s(m_imgExtStr,pImgExt);

			// load the first frame
			char imgNameStr[200];
			sprintf_s(imgNameStr,"%s%s%d%s",
				pImgDirPath,pImgPrefix,0,pImgExt);
			m_pCurImg = cvLoadImage(imgNameStr);
		}

		if (m_pCurImg)
		{
			// create video writer
			m_pVideoWriter = cvCreateVideoWriter(
				pSaveVideoName, // video name
				CV_FOURCC('D','I','V','X'), // video encoding
				30, // frame rates
				cvSize(m_pCurImg->width,m_pCurImg->height)); // frame size
		}
	}

	VideoGen::~VideoGen(void)
	{
		if (m_pCaptureCam)
		{
			cvReleaseCapture(&m_pCaptureCam);
		}

		if (m_pVideoWriter)
		{
			cvReleaseVideoWriter(&m_pVideoWriter);
		}

		if (m_pCurImg && !m_bCam)
		{
			cvReleaseImage(&m_pCurImg);
		}
	}

	bool VideoGen::SaveVideo(unsigned numFrames)
	{
		if (!m_pVideoWriter)
		{
			return false;
		}

		// write the first frame
		cvWriteFrame(m_pVideoWriter,m_pCurImg);
	
		if (m_bCam) // use camera
		{
			while(m_pCurImg = cvQueryFrame(m_pCaptureCam))
			{
				cvWriteFrame(m_pVideoWriter,m_pCurImg);
			}
		}else
		{
			cvReleaseImage(&m_pCurImg); // release the first frame
			char imgNameBuf[200];
			const char * pWinName = "Video Generator";
			cvNamedWindow(pWinName);
			for (int i = 1; i < (int)numFrames; ++i)
			{
				sprintf_s(imgNameBuf,"%s%s%d%s",
					m_imgDirPathStr,m_imgPrefixStr,i,m_imgExtStr);
				m_pCurImg = cvLoadImage(imgNameBuf);
				cvWriteFrame(m_pVideoWriter,m_pCurImg);
				cvShowImage(pWinName,m_pCurImg);
				cvWaitKey(10);
				cvReleaseImage(&m_pCurImg);
			}
			cvDestroyWindow(pWinName);
		}

		return true;
	}
};