#include "stdafx.h"
#include "Recognition.h"

Recognition::Recognition()
	:read_csv_file(false),
	load_sak_file(false),
	train(true),
	path_csv(".\\data\\at.csv"),
	path_train_datas(".\\data\\S_trainDatas.sak"),
	username("Mr.Lee"),
	images(),
	labels(),
	current_label(0),
	current_image(),
	error_msg("��ʱû�д�����Ϣ"),
	model()
{
}

void Recognition::set_path_csv(const std::string &path)
{
	path_csv = path;
}

void Recognition::set_path_train_datas(const std::string &path)
{
	path_train_datas = path;
}

void Recognition::set_username(const std::string &name)
{
	username = name;
}

void Recognition::add_images(const cv::Mat &image)
{
	images.push_back(image);
}

void Recognition::add_labels(const int &number)
{
	labels.push_back(number);
}

void Recognition::set_current_image(const cv::Mat &cur_image)
{
	current_image = cur_image.clone();
}

void Recognition::set_current_image(const std::string &path)
{
	current_image = cv::imread(path, 0);
}

void Recognition::set_current_label(const int &num)
{
	current_label = num;
}

void Recognition::delete_image()
{
	images.pop_back();
}

void Recognition::delete_label()
{
	labels.pop_back();
}

void Recognition::clear_labels()
{
	labels.clear();
}

void Recognition::clear_images()
{
	images.clear();
}

std::string Recognition::get_path_csv() const
{
	return path_csv;
}

std::string Recognition::get_path_train_datas() const
{
	return path_train_datas;
}

std::string Recognition::get_username() const
{
	return username;
}

cv::Mat Recognition::get_current_image() const
{
	return current_image;
}

int Recognition::get_current_label() const
{
	return current_label;
}

std::string Recognition::get_error_msg() const
{
	return error_msg;
}

bool Recognition::need_train() const
{
	return train;
}

void Recognition::initialization()
{
	std::ifstream sak_file(path_train_datas, std::ifstream::in);
	if (!sak_file)	// ���û���Ѿ�ѵ���õ������ļ��ͽ����µ�ѵ��
	{
		std::string tmpname;
		system("cls");
		std::cout << "û��ѵ�����ݣ����������г�ʼ���ȣ�" << std::endl;
		std::cout << "�������û����ƣ�" << std::endl;
		std::cin >> tmpname;
		set_username(tmpname);
		system("cls");
	}
	else
	{
		if (!load_sak_file)
		{
			model = cv::createEigenFaceRecognizer();
			system("cls");
			std::cout << "����ѵ�����ݣ�������..." << std::endl;
			if (!read_csv_file)
			{
				read_csv(path_csv);	// ��ȡ�ļ�����
			}
			model->load(path_train_datas);
			if (images.size() <= 1)	// Ҫ����������ͼƬ
			{
				error_msg = "�ô�ѵ������Ҫ������ͼƬ����";
				print_error_msg();
				return;
			}
			train = false;
			system("cls");
			std::cout << "�������" << std::endl;
			load_sak_file = true;
		}
	}
}

void Recognition::read_csv(const std::string &fileName, const char separator)
{
	std::ifstream csv_file(fileName, std::ifstream::in);
	if (!csv_file)
	{
		error_msg = "�Ҳ�����Ч���ļ�";
		print_error_msg();
		return;
	}
	std::string s_line, s_path, s_number;	// s_lineΪ���е�������Ϣ��s_pathΪ·������Ϣ��s_numberΪ�����Ϣ
	while (std::getline(csv_file, s_line))
	{
		std::stringstream s_getLine(s_line);	//	string��s_getLine�ṩ��string�����������������Ϣ�Ĺ���
		std::getline(s_getLine, s_path, separator);	// ���ӿ�ʼ��";"��������Ϣ����s_path��
		std::getline(s_getLine, s_number);	// ��ʣ���������Ϣ���뵽s_number��
		if (!s_path.empty() && !s_number.empty())
		{
			add_images(cv::imread(s_path, 0));	// ��ȡͼƬ��Ϣ
			add_labels(atoi(s_number.c_str()));	//	��ȡ�����Ϣ�����Ƚ�stringתchar *���ٽ�char *תΪint��atoi������C���Եģ�
		}
	}
	csv_file.close();
	read_csv_file = true;
}

void Recognition::print_error_msg() const
{
	std::copy(error_msg.begin(), error_msg.end(), std::ostream_iterator<const char>(std::cout, ""));
}

int Recognition::predict_label() const
{
	return model->predict(current_image);
}

void Recognition::predict_label(int &label, double &value) const
{
	model->predict(current_image, label, value);
}

void Recognition::start_work()
{
	if (train)
	{// ��Ҫ����ѵ��
		model = cv::createEigenFaceRecognizer();
		system("cls");
		std::cout << "ѵ����..." << std::endl;
		if (!read_csv_file)
		{
			read_csv(path_csv);	// ��ȡ�ļ�����
		}
		model->train(images, labels);	// ѵ��
		system("cls");
		std::cout << "ѵ�����������������..." << std::endl;
		model->save(path_train_datas);	// ����ѵ�����
		train = false;
		system("cls");
		std::cout << "�������" << std::endl;
		return;
	}
	current_label = predict_label();
	if (current_label == 41)	// �����41���û�ָ���ı�ţ�Ҳ����ָ��Ϊ������
	{
		system("cls");
		std::cout << "Hellow, " << username << std::endl;
		current_label = 0;	// ��0
	}
	else
	{
		system("cls");
		std::cout << "�����ޣ�" << std::endl;
		current_label = 0;	//	��0
	}
}