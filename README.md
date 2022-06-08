# Subway Navigation
<div align="center"><img src="https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white" alt="C"></div>

**Subway Navigation finds optimal path for Korean subway navigation. It uses dijkstra algorithm to find the shortest or minimum transfer path**


## How it Works
1. Execute the program by entering `gcc ./subway_navigation.c && ./a.out` to console
<div align="center"><img src="https://github.com/gs97ahn/c_subway_navigation/blob/master/screenshot/step_1.png" alt="screenshot step 1"></div>

2. Enter departure station name
<div align="center"><img src="https://github.com/gs97ahn/c_subway_navigation/blob/master/screenshot/step_2.png" alt="screenshot step 2"></div>

3. Enter destination station name
<div align="center"><img src="https://github.com/gs97ahn/c_subway_navigation/blob/master/screenshot/step_3.png" alt="screenshot step 3"></div>

4. Enter method

4-1. Shortest path
<div align="center"><img src="https://github.com/gs97ahn/c_subway_navigation/blob/master/screenshot/step_4-1.png" alt="screenshot step 4-1"></div>

4-2. Minimum transfer path
<div align="center"><img src="https://github.com/gs97ahn/c_subway_navigation/blob/master/screenshot/step_4-2.png" alt="screenshot step 4-2"></div>

5. Outputs optimal route and estimated time with number of total subway station visits

5-1. Shortest path
<div align="center"><img src="https://github.com/gs97ahn/c_subway_navigation/blob/master/screenshot/step_5-1.png" alt="screenshot step 5-1"></div>

5-2. Minimum transfer path
<div align="center"><img src="https://github.com/gs97ahn/c_subway_navigation/blob/master/screenshot/step_5-2.png" alt="screenshot step 5-2"></div>


## Subway Data
Estimated travel time and estimated transfer time data is available in `data` folder. The data file format is csv.
- 1지선
- 1호선
- 2지선
- 2호선
- 3호선
- 4호선
- 5지선
- 5호선
- 6호선
- 7호선
- 8호선
- 9호선
- 경의선
- 경춘선
- 공항철도
- 분당선
- 인천1선
- 중앙선
- 역이름
- 환승정보


## Exception Handled
- Failed to read csv file
- Unavailable or incorrect station name inputted
- Inputted departure station name and destination station name are matching
- Incorrect method inputted