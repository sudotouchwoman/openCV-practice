#include "Hist.h"
#define RESDIR "C:/sakeofShit/VS/myOpenCVPractice/Resources"
#define SAVEDIR "C:/sakeofShit/VS/myOpenCVPractice/Saves"

int main()
{
	int bin_contrast = 100;
	int bin_intensity = 127;
	const std::string dir(RESDIR);
	const std::string savedir(SAVEDIR);
	
	cv::Mat source = Histo::getData(dir, savedir);
	Histo H(source);
	H.DisplayAll();

	int key = 0;
	int id = 0;

	while ((key = cv::waitKey()) != 27)
	{
		switch (key)
		{
		case 45:
			H.ChangeBrightness(++bin_intensity);
			break;
		case 61:
			H.ChangeBrightness(--bin_intensity);
			break;
		case 95:
			H.ChangeContrast(--bin_contrast);
			break;
		case 43:
			H.ChangeContrast(++bin_contrast);
			break;
		case 115:
			cv::imwrite(savedir + "/" + "Img" + std::to_string(++id) + ".jpg", H.GetSrc());
			cv::imwrite(savedir + "/" + "Hist" + std::to_string(id) + ".jpg", H.GetHist());
			std::cout << "Save done succesfully!\n";
			break;
		case 100:
			Histo::ChangeBrightness(bin_intensity = 127, (void*)&H);
			Histo::ChangeContrast(bin_contrast = 100, (void*)&H);
			std::cout << "Set to default!\n";
			break;
		default:
//			std::cout << "Nothing to do here! KeyCode:\t" << key << "\n";
			break;
		}
	}
	std::cout << "\nProgramm finished.\n";
	return EXIT_SUCCESS;
}