//
// Created by wojkly on 16.04.22.
//
//

#include <iostream>
#include <queue>
#include <string>

using namespace std;


int main() {

    int height, width;
    string line, tmp;

    cin >> tmp;
    height = stoi(tmp);
    cin >> tmp;
    width = stoi(tmp);

//    cout << "height: " << height << " width: " << width << endl;
// skip next 3 lines for checkpoint1
    cin >> tmp;
    cin >> tmp;
    cin >> tmp;

    bool board[height][width];
    bool visited[height][width];
    pair<int,int> parent[height][width];
    
    int s_h = -1, s_w = -1, f_h = -1, f_w = -1;

    for (int i = 0; i < height; ++i) {

        cin >> line;
//        cout << "row " << i << " " << line << endl;
        for (int j = 0; j < width; ++j) {
            visited[i][j] = false;
            switch (line[j]) {
                case '#':
                    board[i][j] = false;
                    break;
                case '.':
                    board[i][j] = true;
                    break;
                case 'a':
                    s_h = i;
                    s_w = j;
                    board[i][j] = true;
                    break;
                case 'A':
                    f_h = i;
                    f_w = j;
                    board[i][j] = true;
                    break;
                default:
                    cout << "zly znak";
                    EXIT_FAILURE;
            }
        }
    }
//    cout << "input done" << endl;
    if (s_h == -1 || s_w == -1 || f_h == -1 || f_w == -1) {
        cout << "nie znaleziono startu / konca trasy";
        EXIT_FAILURE;
    }

    parent[s_h][s_w] = pair<int,int>(-1, -1);

    //BFS
    queue<pair<int,int>> queue;
    queue.push(pair<int,int>(s_h, s_w));
    pair<int,int> cur_pos;
    int new_h, new_w;
    
    pair<int,int> possible_moves[4];
    possible_moves[0] = pair<int,int>(-1,0);
    possible_moves[1] = pair<int,int>(1,0);
    possible_moves[2] = pair<int,int>(0,-1);
    possible_moves[3] = pair<int,int>(0,1);

//    cout << "starting bfs" << endl;
    visited[s_h][s_w] = true;
    while (!queue.empty()) {
        cur_pos = queue.front();
        queue.pop();


        if (cur_pos.first== f_h && cur_pos.second == f_w) {
            break;
        }

        for (auto move: possible_moves) {
            new_h = cur_pos.first + move.first;
            new_w = cur_pos.second + move.second;

            if (0 <= new_h && new_h < height && 0 <= new_w && new_w < width && board[new_h][new_w] && !visited[new_h][new_w]) {
                parent[new_h][new_w] = cur_pos;
                visited[new_h][new_w] = true;
                queue.push(pair<int,int>(new_h, new_w));
            }
        }
    }

//    print_status(parent, height, width);

    //get path
//    cout << "getting path" << endl;
    string result;
    cur_pos = pair<int, int>(f_h, f_w);

    pair<int, int> next_pos = parent[f_h][f_w];

    int h_dif, w_dif, total_diff;
    while(next_pos.first != -1) {
        h_dif = cur_pos.first - next_pos.first;
        w_dif = (cur_pos.second - next_pos.second) * 2;
        total_diff = h_dif + w_dif;
        switch (total_diff) {
            case -1:
                result.append("G");
                break;
            case 1:
                result.append("D");
                break;
            case -2:
                result.append("L");
                break;
            case 2:
                result.append("P");
                break;
            default:
                cout << "wrong move detected";
                EXIT_FAILURE;
        }
//        cout << "cur: " << cur_pos[0] << "," << cur_pos[1] << "next: " << next_pos[0] << "," << next_pos[1] << endl;
        cur_pos = next_pos;
        next_pos = parent[cur_pos.first][cur_pos.second];
    }

    for (int i = result.length() - 1; i >= 0; --i) {
        cout << result[i];
    }
}