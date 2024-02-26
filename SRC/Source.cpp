#include <opencv2\opencv.hpp>

using namespace cv;
#include<iostream>
using namespace std;

#include<vector>
#include<utility>
#include<queue>
#include<string>

struct cmp {
	bool operator()(pair<int, pair<int, int>> a, pair<int, pair<int, int>> b){
		return a.first > b.first;
	}
};

vector<vector<pair<int, int>>> parent2(258, vector<pair<int, int>>(258));
pair<int, int> find_parent(pair<int,int> x) {
	if (parent2[x.first][x.second] == x) {
		return x;
	}
	else {
		return parent2[x.first][x.second] = find_parent(parent2[x.first][x.second]);
	}
}

void main() {
	// string testimg = "lena.tif";
	// string testimg = "caman.tif";
	// string testimg = "nwu.tif";

	
	clock_t start, finish;
	double duration1;
	double duration2;

	//method 1 - priority_queue

	start = clock();

	int cnt1 = 0;

	Mat Image = imread(testimg, 2); // 256*256 image
	imshow("OriginalIMG-pq", Image);

	vector<vector<int>> v(258, vector<int>(258, 1e9));
	vector<vector<bool>> visited(258, vector<bool>(258, 0));
	vector<vector<pair<int, int>>> parent(258, vector<pair<int, int>>(258));
	priority_queue<pair<int, pair<int, int>>,vector<pair<int, pair<int, int>>>,cmp> pq;
	for (int i = 1; i <= 256; i++) {
		for (int j = 1; j <= 256; j++) {
			v[i][j] = int(Image.at<uchar>(Point(i - 1, j - 1)));
		}
	}

	for (int i = 1; i <= 256; i++) {
		for (int j = 1; j <= 256; j++) {
			parent[i][j] = { i,j };
			int window_min = v[i][j];
			for (int a = -1; a <= 1; a++) {
				for (int b = -1; b <= 1; b++) {
					if (v[i + a][j + b] < window_min) {
						window_min = v[i + a][j + b];
					}
				}
			}
			if (v[i][j] == window_min) {
				pq.push({ v[i][j], {i,j} });
				visited[i][j] = 1;
				cnt1++;
			}
		}
	}

	for (int i = 0; i < 258;i++) {
		visited[i][0] = 1;
		visited[i][257] = 1;
		visited[0][i] = 1;
		visited[257][i] = 1;
	}
	
	while (!pq.empty()) {
		pair<int, pair<int, int>> x = pq.top();
		pair<int, int> idx = x.second;
		int val = x.first;
		pq.pop();
		for (int a = -1; a <= 1; a++) {
			for (int b = -1; b <= 1; b++) {
				if (!visited[idx.first + a][idx.second + b]) {
					pq.push({v[idx.first + a][idx.second + b],{idx.first+a,idx.second+b}});
					parent[idx.first+a][idx.second+b] = parent[idx.first][idx.second];
					visited[idx.first + a][idx.second + b] = 1;
				}
			}
		}
	}

	for (int i = 1; i <= 256; i++) {
		for (int j = 1; j <= 256; j++) {
			Image.at<uchar>(Point(i-1, j-1)) = Image.at<uchar>(Point(parent[i][j].first-1,parent[i][j].second-1));
		}
	}
	imshow("ResultIMG-pq", Image);

	finish = clock();
	duration1 = (double)(finish - start) / CLOCKS_PER_SEC;
	
	




	//method 2 - union-find
	start = clock();
	int cnt2 = 0;
	Mat Image2 = imread(testimg, 2); // 256*256 image
	imshow("OriginalIMG-uf", Image2);

	vector<vector<int>> v2(258, vector<int>(258, 1e9));

	for (int i = 1; i <= 256; i++) {
		for (int j = 1; j <= 256; j++) {
			v2[i][j] = int(Image2.at<uchar>(Point(i - 1, j - 1)));
		}
	}

	for (int i = 1; i <= 256; i++) {
		for (int j = 1; j <= 256; j++) {
			parent2[i][j] = make_pair(i,j);
			int window_min = v[i][j];
			for (int a = -1; a <= 1; a++) {
				for (int b = -1; b <= 1; b++) {
					if (v[i + a][j + b] < window_min) {
						window_min = v[i + a][j + b];
						parent2[i][j] = { i + a,j + b };
					}
				}
			}
			if (parent2[i][j] == make_pair(i, j)) {
				cnt2++;
			}
		}
	}
	for (int i = 1; i <= 256; i++) {
		for (int j = 1; j <= 256; j++) {
			find_parent({ i,j });
		}
	}
	for (int i = 1; i <= 256; i++) {
		for (int j = 1; j <= 256; j++) {
			Image2.at<uchar>(Point(i - 1, j - 1)) = Image2.at<uchar>(Point(parent2[i][j].first - 1, parent2[i][j].second - 1));
		}
	}

	imshow("ResultIMG-uf", Image2);
	finish = clock();
	duration2 = (double)(finish - start) / CLOCKS_PER_SEC;


	cout << "Method1(Priority_Queue): "<<duration1 << "sec" << endl;
	cout << "#of segments: " << cnt1 << endl;
	cout << "Method2(Union-Find): "<<duration2 << "sec" << endl;
	cout << "#of segments: " << cnt2 << endl;
	waitKey(0);
	return;
}