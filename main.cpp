#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void MotionDetect(); // Function to Capture Video from Cam

int main()
{
	MotionDetect();
	exit(0);
}

void MotionDetect() // Capture Video of QR Code
{
	Mat frame, ref_frame, difference, gray, channel[3];
	int count = 5;												// Holds assumed frame count

	VideoCapture camera(0);

	// Sets width and height of window to 512 and 288 respectively to enable faster processing of video
	camera.set(3, 512);
	camera.set(4, 288);

	// If Camera failed to open exit the function
	if (!camera.isOpened())
	{
		cout << "Could not capture Video.\n";
		exit(1);
	}

	//Reads first frame
	camera.read(ref_frame);
	cvtColor(ref_frame, ref_frame, COLOR_BGR2GRAY);
	GaussianBlur(ref_frame, ref_frame, Size(5, 5), 0);

	//Reads subsequent frames 
	while (camera.read(frame))
	{
		imshow("Original Video", frame);

		//Detects motion by comparing current frame with reference frame and store result in difference
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gray, Size(5, 5), 0);
		absdiff(ref_frame, gray, difference);
		threshold(difference, difference, 25, 255, THRESH_BINARY);
		dilate(difference, difference, Mat(), Point(-1, -1), 2);
		imshow("Diff", difference);

		// Difference frame will be black unless there is motion. 
		if (countNonZero(difference))
		{
			cout << "Motion Detected\n";
		}

		// Reinitialise reference frame once every 5 frames
		if (count % 5 == 0)
		{
			frame.copyTo(ref_frame);
			cvtColor(ref_frame, ref_frame, COLOR_BGR2GRAY);
			GaussianBlur(ref_frame, ref_frame, Size(5, 5), 0);
		}
		count++;

		// Stop when escape key is pressed
		if (waitKey(1) == 27)
			break;
	}

	camera.release();
	destroyAllWindows();
}
