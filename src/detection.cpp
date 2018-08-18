#include "detection.h"
#include "yolo_v2_class.hpp"
#include <stdio.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <thread>
#include <atomic>
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <sys/time.h>
#include "globals.h"

#include "yolo_v2_class.hpp"    // imported functions from DLL

std::vector<std::string> objects_names_from_file(std::string const filename) {
	std::ifstream file(filename);
	std::vector<std::string> file_lines;
	if (!file.is_open()) return file_lines;
	for (std::string line; getline(file, line);) file_lines.push_back(line);
	std::cout << "object names loaded \n";
	return file_lines;
}

std::vector<bbox> build_bbox(std::vector<bbox_t> const result_vec, std::vector<std::string> const obj_names) {

	std::vector<bbox> detections;
	int eyeCounter = 0;
	for (auto &i : result_vec) {
		bbox detection = { 0, 0, 0, 0, 0, 0, none};
		detection = {
			i.x,
			i.y,
			i.w,
			i.h,
			i.x + i.w / 2,
			i.y + i.h / 2,
			obj_names[i.obj_id] == "face" ? face : eye
		};

		if(	detection.type == eye) eyeCounter++;

		if(eyeCounter==2){
			if(detections[0].type==eye){
				if(detections[0].left<detection.left){
					detection.type = right_eye;
					detections[0].type = left_eye;
				}
				else {
					detections[0].type = right_eye;
					detection.type = left_eye;
				}
			}
			else if (detections[1].type == eye ){
				if(detections[1].left<detection.left){
					detection.type = right_eye;
					detections[1].type = left_eye;
				}
				else {
					detections[1].type = right_eye;
					detection.type = left_eye;}
			}
		}
		// std::cout << obj_names[i.obj_id] << " - ";
		// std::cout << "obj_id = " << i.obj_id << ",  x = " << i.x << ", y = " << i.y
		//           << ", w = " << i.w << ", h = " << i.h
		//           << std::setprecision(3) << ", prob = " << i.prob << std::endl;
		detections.push_back(detection);

	}

	return detections;
}

double what_time_is_it_now()
{
	struct timeval time;
	if (gettimeofday(&time, NULL)) {
		return 0;
	}
	return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

std::vector<bbox> detect_object(Mat frame, std::vector<std::string> obj_names) {
	try {
		double time = what_time_is_it_now();
		std::vector<bbox_t> result_vec = detector.detect(frame, 0.1);
		// printf("predicted in: %f\n", what_time_is_it_now() - time);
		return build_bbox(result_vec, obj_names);
	}
	catch (std::exception &e) { std::cerr << "exception: " << e.what() << "\n"; getchar(); }
	catch (...) { std::cerr << "unknown exception \n"; getchar(); }
}