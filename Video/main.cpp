#include "Video.h"
static void PrintMsg(void);

int main(int argc, char *args[])
{
	// declarations
	using video::VideoGen;
	using video::VideoDiv;
	using std::cout;
	using std::endl;
	using std::cerr;
	using std::string;

	// parse input string
	if (argc == 1) // no input argument
	{
		PrintMsg();
	}else
	{
		const char * const option = args[1];
		if (!strcmp(option,"-h")) // print help msg
		{
			PrintMsg();
		}else if (!strcmp(option,"-d")) // divide video into images
		{
			if (4 > argc)
			{
				cerr << "Invalid number of arguments." << endl;
				return EXIT_FAILURE;
			}else
			{
				const char * const src = args[2];
				const char * const dst = args[3];
				VideoDiv * pVDiv = NULL;
				if (!strcmp(src,"0"))// use camera
				{
					pVDiv = new VideoDiv(NULL);
				}else
				{
					pVDiv = new VideoDiv(src);
				}
				unsigned num = pVDiv->SaveImages(dst);
				delete pVDiv;
				cout << "Save " << num << "images in "<< dst << endl;
			}
		}else if (!strcmp(option,"-r")) // record video from images 
		{
			if (4 > argc)
			{
				cerr << "Invalid number of arguments." << endl;
				return EXIT_FAILURE;
			}else
			{
				const char * const src = args[2];
				const char * const dst = args[3];
				unsigned num = 0; // number of frames to be recorded

				VideoGen * pVGen = NULL;
				if (!strcmp(src,"0")) // use camera
				{
					pVGen = new VideoGen(NULL,dst);
				}else
				{
					if (5 > argc)
					{
						cerr << "Not enough arguments." << endl;
						return EXIT_FAILURE;
					}
					const char * const numStr = args[4];
					num = (unsigned)atoi(numStr);

					// split path
					char drive[10],dir[150],name[50],ext[10];
					_splitpath_s(src,drive,dir,name,ext);
					// eliminate the last 0
					name[strlen(name)-1] = '\0';
					std::ostringstream oss;
					oss << drive << dir;
					string imgNamePre = oss.str();
					char imgNameCStr[150];
					strcpy_s(imgNameCStr,imgNamePre.c_str());
					pVGen = new VideoGen(imgNameCStr,dst,name,ext);
				}
				bool success = pVGen->SaveVideo(num);
				delete pVGen;
				if (success)
				{
					cout << "Record video at " << dst << endl;
				}else
				{
					return EXIT_FAILURE;
				}
			}
		}else
		{
			cerr << "Unknown arguments." << endl;
			PrintMsg();
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

// Print help info
static void PrintMsg(void)
{
	using std::cout;
	using std::endl;

	cout << "Usage: Divide a video into images and generate a video from \
		images or camera" << endl;
	cout << "Video [options] src dst num" << endl;
	cout << "-h\tPrint help message\n";
	cout << "-d\tDivide video into images. src=source video path, and 0\
			means using camera (press ESC to exit recording). dst=save path\n";
	cout << "-r\tRecord video from images or camera. src=path of the first\
			image(start with image0.jpg), and 0 means using camera (press \
			ESC to exit recording). dst=save path\n" << std::flush;

}