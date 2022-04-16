//
// Created by wojkly on 16.04.22.
//
//
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

using namespace std;


void print_status(int*** parents, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cout << "(" << parents[i][j][0] << "," << parents[i][j][1] << ") ";
        }
        cout << endl;
    }
    cout << endl;
}

string find_path(int*** parent, int f_h, int f_w) {
    string result;
    int* cur_pos = new int[2];
    cur_pos[0] = f_h;
    cur_pos[1] = f_w;

    int* next_pos = parent[f_h][f_w];

    int h_dif, w_dif;
    while(next_pos[0] != -1) {
        h_dif = cur_pos[0] - next_pos[0];
        w_dif = cur_pos[1] - next_pos[1];
        if (h_dif == -1) {
            result.append("G");
        } else if (h_dif == 1) {
            result.append("D");
        } else if (w_dif == -1) {
            result.append("L");
        } else if (w_dif == 1) {
            result.append("P");
        }
//        cout << "cur: " << cur_pos[0] << "," << cur_pos[1] << "next: " << next_pos[0] << "," << next_pos[1] << endl;
        cur_pos = next_pos;
        next_pos = parent[next_pos[0]][next_pos[1]];
    }
//    delete cur_pos;

    int n = result.length() - 1;
    for(int i=0;i<(result.length()/2);i++){
        //Using the swap method to switch values at each index
        swap(result[i],result[n]);
        n = n-1;

    }
    return result;
}

string find_min_distance_path(bool **board, int height, int width, int s_h, int s_w, int f_h, int f_w) {

    bool** visited = new bool*[height];
    int*** parent = new int**[height];
    for (int i = 0; i < height; ++i) {
        visited[i] = new bool[width];
        parent[i] = new int*[width];
        for (int j = 0; j < width; ++j) {
            visited[i][j] = false;
            parent[i][j] = new int[2];
            parent[i][j][0] = -1;
            parent[i][j][1] = -1;
        }
    }

    queue<int*> queue;

    int* start_pos = new int[2];
    start_pos[0] = s_h;
    start_pos[1] = s_w;
    queue.push(start_pos);

    while (!queue.empty()) {
        int* cur_pos = queue.front();
        queue.pop();

        visited[cur_pos[0]][cur_pos[1]] = true;

        if (cur_pos[0] == f_h && cur_pos[1] == f_w) {
            break;
        }

        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if (abs(i + j) == 1) {
                    int new_h, new_w;
                    new_h = cur_pos[0] + i;
                    new_w = cur_pos[1] + j;

                    int* new_pos = new int[2];
                    new_pos[0] = new_h;
                    new_pos[1] = new_w;

                    if (0 <= new_h && new_h < height && 0 <= new_w && new_w < width && board[new_h][new_w] && !visited[new_h][new_w]) {
                        parent[new_h][new_w][0] = cur_pos[0];
                        parent[new_h][new_w][1] = cur_pos[1];
                        queue.push(new_pos);
                    }
                }
            }
        }

        delete[] cur_pos;
    }

//    print_status(parent, height, width);

    string result = find_path(parent, f_h, f_w);

    for (int i = 0; i < height; ++i) {
        delete[] visited[i];
        for (int j = 0; j < width; ++j) {
            delete[] parent[i][j];
        }
        delete[] parent[i];
    }
    delete[] visited;
    delete[] parent;

    return result;
}

int main() {

    int height, width;
    string line, tmp;

    cin >> tmp;
    height = stoi(tmp);
    cin >> tmp;
    width = stoi(tmp);

//    cout << "height " << height << " width " << width << endl;

// skip next 3 lines for checkpoint1
    cin >> tmp;
    cin >> tmp;
    cin >> tmp;

    bool** board = new bool*[height];
    for (int i = 0; i < height; ++i) {
        board[i] = new bool[width];
    }
    
    int s_h, s_w, f_h, f_w;

    for (int i = 0; i < height; ++i) {
        cin >> line;
//        cout << "row " << i << " " << line << endl;
        for (int j = 0; j < width; ++j) {
            if (line[j] == '.') {
                board[i][j] = true;
            } else if (line[j] == '#') {
                board[i][j] = false;
            } else if (line[j] == 'a') {
                s_h = i;
                s_w = j;
                board[i][j] = true;
            } else if (line[j] == 'A') {
                f_h = i;
                f_w = j;
                board[i][j] = true;
            }
        }
    }

    string result = find_min_distance_path(board, height, width, s_h, s_w, f_h, f_w);

    for (int i = 0; i < height; ++i) {
        delete[] board[i];
    }
    delete[] board;

    cout << result;
}