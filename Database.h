#pragma once

#include <mysql.h>
#include <string>
#include <vector>

std::vector<std::pair<std::string, int>> getTopScores(MYSQL* conn);
void saveScore(MYSQL* conn, const std::string& name, int score);
MYSQL* connectToDatabase();

