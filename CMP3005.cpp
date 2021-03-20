#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <set>
#include <unordered_map>
#include <sstream>
#include <map>


namespace hash_space
{
	namespace hash::fnv   // fnv is an hashing algorithm (Fowler–Noll–Vo hash function) 
	{
		class HashObj
		{
		public:
			using hash_type = std::size_t; //size_t to represents the hash value of the parameter
			const hash_type default_offset_basis = 0x811C9DC5; //FNV parameters hexadecimal representation -> 32 bits
			const hash_type prime = 0x01000193; // FNV parameters hexadecimal representation -> 32 bits  
			hash_type operator()(const std::string& line) const
			{
				hash_type hash_val = default_offset_basis;   // created a hash_val with the parameter type
				for (const auto& item : line) // for each item  in the line input
				{
					hash_val *= prime;
					hash_val ^= static_cast<hash_type>(item);
				}
				return hash_val;
			}
		};
	}
}
auto get_frequency(const std::vector<std::string>& words)
{
	std::unordered_map<std::string, int, hash_space::hash::fnv::HashObj> hash_table{}; //we stored 3 value in our undored map, we used undored map to store each value kept according to the order we add
	for (const auto& item : words)													// hash_table value starts from empty state {}
	{
		++hash_table[item];   // item al +1
	}
	return hash_table;
}

double dot_product(std::vector<int> a, std::vector<int> b) {	// we calculated the product of 2 vectors
																
	double sum = 0;
	for (int i = 0; i < a.size(); i++)
		sum += static_cast<double>(a[i]) * static_cast<double>(b[i]);
	return sum;
}
double norm_calculate(std::vector<int> a, std::vector<int> b) {	// we calculated square of each one and take their sum then 
																// apply it  for the second vector, finally take there sqrt and multiply them
																
	double normA = 0;
	double normB = 0;

	for (int i = 0; i < a.size(); i++)
		normA += pow(static_cast<double>(a[i]), 2);

	for (int i = 0; i < b.size(); i++)
		normB += pow(static_cast<double>(b[i]), 2);


	return (sqrt(normA) * sqrt(normB));
}

double cosine_score(const std::vector<int>& first_vector, const std::vector<int>& second_vector) {   // to calculate cosine score we  used (dc deki formul) and 
																									//apply it to 2 integer vectors first and second vector
	return dot_product(first_vector, second_vector) /
		norm_calculate(first_vector, second_vector);
}


auto cosine_similarity(const std::vector<std::string>& first_text, const std::vector<std::string>& second_text)
{
	/* Recording the starting clock tick.*/
	clock_t start, end;
	start = clock();

	std::set<std::string> words;
	words.insert(first_text.begin(), first_text.end());
	words.insert(second_text.begin(), second_text.end());

	auto first_text_freq = get_frequency(first_text);
	auto second_text_freq = get_frequency(second_text);

	std::vector<int> first_text_cosine_vector;
	std::vector<int> second_text_cosine_vector;

	for (const auto& word : words)
	{
		if (auto search{ first_text_freq.find(word) }; search != first_text_freq.end()) // end bulunamamıs, end değil ise bulunmuş
			first_text_cosine_vector.push_back(search->second);   // pair string, int -> int check
		else
			first_text_cosine_vector.push_back(0);

		if (auto search{ second_text_freq.find(word) }; search != second_text_freq.end())
			second_text_cosine_vector.push_back(search->second);  // pair string, int -> int check
		else
			second_text_cosine_vector.push_back(0);
	}

	return 100. * cosine_score(first_text_cosine_vector, second_text_cosine_vector);  // cosine vektor her kelime için tekrarlanıyorsa frequencysi, tekrarlanmıyorsa 0 değerini tutan vektör.
}

auto get_words(const std::string& arg)
{
	std::string text{ arg }; // created text string to store arg value in it 
	text.erase(std::remove_if(text.begin(), text.end(), [](auto val) //if each value in the text include punctuation except point then remove all
	{
		if (val < 0 || val > 255) return true; // if it is an only char variable return true
		return std::ispunct(static_cast<int>(val)) && val != '.';
	}), text.end());

	text.erase(std::unique(text.begin(), text.end(), [](auto val, auto val2) // if there exists more than 1 space then delete othersi, leave 1
	{
		if (val < 0 || val > 255) return true;
		return std::isspace(static_cast<int>(val)) && std::isspace(static_cast<int>(val2));
	}), text.end());

	std::stringstream breakwords{ text }; //we used stringstream to break input into word 
	std::string t; //To store individual words
	std::vector<std::string> newform;

	while (getline(breakwords, t, ' '))                     
	{
		newform.emplace_back(t);    // get each word in breakwords and add it newform string vector
	}
	return newform;
}

auto get_lines_as_words(const std::string& arg)  // sentence al word ayır 
{
	// ??? cumle mi kelime mi

	std::vector<std::vector<std::string>> line_similarities;
	std::stringstream breakwords{ arg };
	std::string t;
	while (std::getline(breakwords, t, '.'))
	{
		line_similarities.push_back(get_words(t));
	}
	return line_similarities;
}


int main() {
	std::string directory_path;
	std::cout << "Enter a directory path : ";
	getline(std::cin, directory_path);

	// we created directory path as a String variable (C://********) to reach folder by using it.
	// we get folder's by path as an input from the user.
	// path of folder = directory_path

	if (!(std::filesystem::exists(directory_path) && std::filesystem::is_directory(directory_path)))
	{
		std::cerr << "Please enter a directory path!\n";
		std::exit(1);
	}

	//after the user enter the path, if it exists in the computer file system  and also if it is an real path variable, we ll pabreakwords to next step
	//if not then the procebreakwords ll stop and user will get an output saying  "Please enter a directory path!"

	std::string target_file;

	std::cout << "Enter a target file : ";
	getline(std::cin, target_file);

	// we created target file as a String variable (***) to reach folder.
	// we get folder name  as an input from the user.
	// folder name = target_file

	if (!(std::filesystem::exists(target_file) && std::filesystem::is_regular_file(target_file)))
	{
		std::cerr << "Please enter a regular file path!\n";
		std::exit(1);
	}
	//after the user enter the file name, if it exists in the computer file system  and also if it is an one type of file stored 
	//in a file system, we ll pabreakwords to next step
	//if not then the procebreakwords ll stop and user will get an output saying  "Please enter a regular file path!"


	std::ifstream targetf{ target_file }; //we open target_file in read mood and named targetf



	if (!targetf.is_open())
	{
		std::cerr << "File could not be opened.\n";
		std::exit(1);
	}

	// if targetf cannot be opened then the system stops the procebreakwords and the output will be "File could not be opened"

	std::filesystem::directory_iterator directory_iter{ directory_path };

	// we created directory_iter as an iterator for directory_path in the file system.

	std::vector<std::ifstream> source_files;

	// created a vector which includes ifstreams , named source_files

	std::for_each(begin(directory_iter), end(directory_iter), [&source_files](auto val)
	{// for each data in directory_iter that we created above to collect directory_path , we take each val

		if (val.is_regular_file()) {   // and we compare it with the format of regular file properties.If it is
			source_files.emplace_back(val.path()); //we push that file path into source_file vector.
			//****** we used emplace_back rather then push_back, because it does relies on compiler optimizations
			//to avoid copies and also it a good way to send the arguments directly to the constructor to create
			//an object in-place.
			if (!source_files.back().is_open()) //if the last data in source_files is not reachable
			{
				// then the system stops the procebreakwords and the output will be "One of the file in directory could not be opened."
				std::cerr << "One of the file in directory could not be opened.\n";
				std::exit(1);
			}
		}
	});

	std::vector<std::string> source_documents; // created a vector to add String variables and named source_documents
	std::string target_document = "";// created String variables 
	std::string temp = "";// created String variables 

	while (getline(targetf, temp))
		//in target file (targetf) vector, for each string we add to target_document, initially target_document is null and till the
	// en of targetf we create a string target_document
	{
		target_document += temp;
	}

	for (auto& item : source_files) //item is a reference to an item of vector source_file                     
	{
		source_documents.emplace_back();   
		while (getline(item, temp))        // boş bi yer allocate ediyorz emplace_back ile . en sonda olduğu biliniyer 
			// we allocate space in source_documents with emplace_back 
		{
			source_documents.back() += temp;
		}
	}

	for (auto& item : target_document)
	{
		if (item < 0 || item > 255) item = ' ';  //ASCII is an 8 bit character set
		//2^8 = 256 with counting starts with 0 then its maximum value is  255, if it is
		//smaller than 0 and exceed 255 bit then item should be empty
		if (std::isupper(item))  // if the items in target_document is in Upper form then transform to lower as char variable  and named to item. 
		{
			item = static_cast<char>(std::tolower(item)); // cast string to char
		}
	}

	for (auto& outer_item : source_documents)
	{
		for (auto& item : outer_item)
		{
			if (item < 0 || item > 255) item = ' ';       //same thing

			if (std::isupper(item))
			{
				item = static_cast<char>(std::tolower(item));
			}
		}
	}

	std::vector<std::vector<std::string>> document_words;  // document_words vector represents  Kelimeleri tutan dokuman vectorunu tutuyo as a string variable vector in the file vector 
	std::vector<std::vector<std::vector<std::string>>> document_text_line_words;// document_text_line_words vector represents  words as a string (cumle)
	//variable vector in the document_words (sentences) vector

	for (auto& outer_item : source_documents)
	{
		document_words.push_back(get_words(outer_item));  // get returned value from get_words "newform" and push it to document_words string vector
		document_text_line_words.push_back(get_lines_as_words(outer_item));
	}

	auto target_text_words{ get_words(target_document) }; // get_words den dönen değer target_text_words 

	std::vector<std::pair<std::vector<std::string>, std::string>> target_text_line_words; // 2 değerli vector ----    vector, string

	std::stringstream breakwords{ target_document };
	std::string t;

	while (std::getline(breakwords, t, '.'))
	{
		target_text_line_words.emplace_back(get_words(t), t);
	}

	int cnt{};

	/* Recording the starting clock tick.*/
	clock_t start, end;
	start = clock();
	for (std::size_t i = 0; i < document_words.size(); ++i)
	{
		std::map<std::string, double> most_similar_sentences;
		for (const auto& line : target_text_line_words)
		{
			for (const auto& item : document_text_line_words[i])
			{
				auto similarity{ cosine_similarity(line.first, item) };
				if (auto find = most_similar_sentences.find(line.second); find != most_similar_sentences.end())
				{
					if (find->second < similarity)
					{
						most_similar_sentences[line.second] = similarity;
					}
				}
				else
				{
					most_similar_sentences.insert(std::make_pair(line.second, similarity));
				}
			}
		}
		int counter_sentence{};
		for (const auto& item : most_similar_sentences)
		{
			if (item.second > 89) // ıt can change / smilarity rate
			{
				++counter_sentence;
			}
		}
		std::cout << std::setprecision(4) << cnt++ << ". Document Similarity : %" << (static_cast<double>(counter_sentence) / most_similar_sentences.size()) * 100 << "\n";
		std::vector<std::pair<std::string, double>> elements{};
		for (auto& item : most_similar_sentences)
		{
			elements.emplace_back(item.first, item.second);
		}
		std::sort(elements.begin(), elements.end(), [](const auto& val1, const auto& val2)
		{
			return val1.second > val2.second;
		});
		int counter{};
		for (const auto& item : elements)
		{
			std::cout << std::setprecision(4) << ++counter << ". Similar Sentence : " << item.first << " - Similarity Rate : %" << item.second << "\n";
			if (counter >= 5) break;
		}
	}
}
