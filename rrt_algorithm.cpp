#include "stdio.h"
#include <stdlib.h>
#include <vector>
#include <math.h>


using namespace std;

typedef struct
{
    int x;
    int y;
}coordi;

struct Node {
    vector<Node *> children;
    Node *parent;
    coordi position;
};

Node start_node;
Node end_node;
Node* nodes[5000];
int startX;
int startY;
int endX;
int endY;
int totnodes = 0;
int reached = 0;
int step_size = 10;
int iter = 0;
int nodeSize = 5;

SDL_Rect* obs = new SDL_Rect[numObs];

const int numObs = 3;


SDL_Window* window;
SDL_Renderer* _renderer;
int _screenWidth;
int _screenHeight;
GameState _gamestate;

void redrawSF() {
    //Starting point
    SDL_Rect startRect;
    startRect.h = nodeSize;
    startRect.w = nodeSize;
    startRect.x = startX-nodeSize/2;
    startRect.y = startY - nodeSize / 2;
    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(_renderer, &startRect);

    //Finish point
    SDL_Rect finRect;
    finRect.h = nodeSize;
    finRect.w = nodeSize;
    finRect.x = endX - nodeSize / 2;
    finRect.y = endY - nodeSize / 2;
    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(_renderer, &finRect);

    SDL_RenderPresent(_renderer);
}

void createScreen(){
    SDL_Init(SDL_INIT_EVERYTHING);
    _window = SDL_CreateWindow("Probabilistic Roadmap", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_SHOWN);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (_window == nullptr) {
        fatalError("SDL Window could not be created!");
    }
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    SDL_RenderClear(_renderer);
}

void connect(int x, int y, int x2, int y2){
    SDL_RenderDrawLine(_renderer, x, y, x2, y2);
    redrawSF();
}

void MainGame::createObstacle() {

    obs[0].h = 180;
    obs[0].w = 40;
    obs[0].x = 160;
    obs[0].y = 50;

    obs[1].h = 180;
    obs[1].w = 40;
    obs[1].x = 330;
    obs[1].y = 50;

    obs[2].h = 40;
    obs[2].w = 130;
    obs[2].x = 200;
    obs[2].y = 120;

    SDL_SetRenderDrawColor(_renderer, 75, 0, 130, 255);
    for (int i = 0; i < numObs; i++) {
        SDL_RenderFillRect(_renderer, &obs[i]);
    }
    SDL_RenderPresent(_renderer);
}

int maxNum(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}

//Returns min of two integers
int minNum(int a, int b) {
    if (a < b)
        return a;
    else
        return b;
}

void init()
{
    start_node.position.x = 50;
    start_node.position.y = 50;
    startX = 50;
    startY = 50;
    start_node.parent = NULL;
    for(int i=start_node.position.x - 5; i < start_node.position.x + 5; i++)
    {
        for(int j=start_node.position.y - 5; j < start_node.position.y + 5; j++)
        {
            //color the start node
        }
    }
    nodes[totnodes++] = &start_node;
    end_node.position.x = 350;
    end_node.position.y = 350;
    endX = 350;
    endY = 350;
    redrawSF();
    for(int i=end_node.position.x - 5; i < end_node.position.x + 5; i++)
    {
        for(int j=end_node.position.y - 5; j < end_node.position.y + 5; j++)
        {
            //color the end node
        }
    }
    createScreen();
    srand(time(NULL));

}

float node_dist(coordi p, coordi q)
{
    coordi v;
    v.x = p.x - q.x;
    v.y = p.y - q.y;
    return sqrt(pow(v.x, 2) + pow(v.y, 2));
}
int near_node(Node rnode)
{
    float min_dist = 999.0, dist= node_dist(start_node.position, rnode.position);
    int lnode = 0, i = 0;
    for(i=0; i<totnodes; i++)
    {
        dist = node_dist(nodes[i]->position, rnode.position);
        if(dist<min_dist)
        {
            min_dist = dist;
            lnode = i;
        }
    }
    return lnode;
}
coordi stepping(coordi nnode, coordi rnode)
{
    coordi interm, step;
    float magn = 0.0, x = 0.0, y = 0.0;
    interm.x = rnode.x - nnode.x;
    interm.y = rnode.y - nnode.y;
    magn = sqrt((interm.x)*(interm.x) + (interm.y)*(interm.y));
    x = (float)(interm.x / magn);
    y = (float)(interm.y / magn);
    step.x = (int)(nnode.x + step_size*x);
    step.y = (int)(nnode.y + step_size*y);
    return step;
}
//int check_validity_1(coordi p, coordi q)
//{
//    coordi large, small;
//    int i = 0, j1 = 0, j2 = 0;
//    double slope;
//    if(q.x<p.x)
//    {
//        small = q;
//        large = p;
//    }
//    else
//    {
//        small = p;
//        large = q;
//    }
//    if(large.x == small.x)
//        return 0;
//    slope = ((double)large.y - small.y)/((double)large.x - small.x);
//    for(i=small.x+1; i<large.x; i++)
//    {
//        j1 = (int)((i*slope) - (small.x)*(slope) + small.y);
//        j2 = j1 + 1;
//        if((i<0) || (i>400) || (j1<0) || (j1>400) || (j2<0) || (j2>400))
//            continue;
////if(((int)img .at<Vec3b>(i, j1)[0] <250) && ((int)img.at<Vec3b>(i, j1)[1] < 250) && ((int)img.at<Vec3b>(i, j1)[2] < 250))
////        if(obstacle)
////            return 0;
////        if(((int)img.at<Vec3b>(i, j2)[0] <250) && ((int)img.at<Vec3b>(i, j2)[1] < 250) && ((int)img.at<Vec3b>(i, j2)[2] < 250))
////        if(osbtale)
////            return 0;
//
//    }
//    return 1;
//}
//int check_validity_2(coordi p, coordi q)
//{
//    coordi large, small;
//    int i = 0, j1 = 0, j2 = 0;
//    double slope;
//    if(q.y<p.y)
//    {
//        small = q;
//        large = p;
//    }
//    else
//    {
//        small = p;
//        large = q;
//    }
//    if(large.x == small.x)
//        return 0;
//    slope = ((double)large.y - small.y)/((double)large.x - small.x);
//    for(i=small.y+1; i<large.y; i++)
//    {
//        j1 = (int)(((i - small.y)/slope) + small.x);
//        j2 = j1 + 1;
//        if((i<0) || (i>400) || (j1<0) || (j1>400) || (j2<0) || (j2>400))
//            continue;
////        if(((int)img.at<Vec3b>(j1, i)[0] <250) && ((int)img.at<Vec3b>(j1, i)[1] < 250) && ((int)img.at<Vec3b>(j1, i)[2] < 250))
////            return 0;
////        if(((int)img.at<Vec3b>(j2, i)[0] <250) && ((int)img.at<Vec3b>(j2, i)[1] < 250) && ((int)img.at<Vec3b>(j2, i)[2] < 250))
////            return 0;
//    }
//    return 1;
//}

bool notObstructed(int x1, int y1, int x2, int y2) {
    //Check each obstacle
    for (int i = 0; i < numObs; i++) {
        //Horizontal line check
        if ((y1 - y2) == 0 && (
                (obs[i].x <maxNum(x1,x2) && obs[i].x>minNum(x1,x2)&&y1>obs[i].y && y1<obs[i].y+obs[i].h)
                ||(obs[i].x+obs[i].w<maxNum(x1,x2) && obs[i].x + obs[i].w>minNum(x1, x2) && y1>obs[i].y && y1<obs[i].y + obs[i].h)) ){
            return false;
        }
            //Vertical line check
        else if ((x1 - x2) == 0 && (
                (obs[i].y <maxNum(y1, y2) && obs[i].y>minNum(y1, y2) && x1>obs[i].x && x1<obs[i].x + obs[i].w)
                || (obs[i].y +obs[i].h<maxNum(y1, y2) && obs[i].y+obs[i].h>minNum(y1, y2) && x1>obs[i].x && x1<obs[i].x + obs[i].w)
        )) {
            return false;
        }
            //Regular line check
        else {
            //Check each equation for each edge of rectangle
            float dX = (float)(x2 - x1);
            float dY = (float)(y2 - y1);
            float slope = dY / dX;
            float a = (float)(obs[i].x);
            float b = (float)(obs[i].x+obs[i].w);
            float c = (float)(obs[i].y);
            float d = (float)(obs[i].y + obs[i].h);

            if ( ((((slope*(a-x1)+y1) > obs[i].y) && ((slope*(a - x1) + y1) < obs[i].y+obs[i].h)) && (a<=maxNum(x1,x2) && a>=minNum(x1,x2)))
                 || ((((slope*(b - x1) + y1) > obs[i].y) && ((slope*(b - x1) + y1) < obs[i].y + obs[i].h)) && (b <= maxNum(x1, x2) && b >= minNum(x1, x2)))
                 || (((((c - y1)/slope + x1) > obs[i].x) && ((c - y1)/slope + x1) < obs[i].x + obs[i].w) && (c <= maxNum(y1, y2) && c >= minNum(y1, y2)))
                 || (((((d - y1) / slope + x1) > obs[i].x) && ((d - y1) / slope + x1) < obs[i].x + obs[i].w) && (d <= maxNum(y1, y2) && d >= minNum(y1, y2)))) {

                return false;
            }

        }
    }
    return true;
}

void draw_path()
{
    Node up, down;
    int breaking = 0;
    down = end_node;
    up = *(end_node.parent);
    while(1)
    {
//        line(img, Point(up.position.y, up.position.x), Point(down.position.y, down.position.x), Scalar(0, 255, 0), 2, 8);
        connect(up.position.y, up.position.x, down.position.y, down.position.x);
//        DRAW THE LINE THAT CONNECTS UP AND DOWN
        if(up.parent == NULL)
            break;
        up = *(up.parent);
        down = *(down.parent);
    }
}

void rect(int x, int y){
    SDL_Rect nodeRect;
    nodeRect.h = nodeSize;
    nodeRect.w = nodeSize;
    nodeRect.x = x-nodeSize/2;
    nodeRect.y = y-nodeSize/2;
    SDL_RenderFillRect(_renderer, &nodeRect);
    redrawSF();
    SDL_RenderPresent(_renderer);
}

void rrt()
{
    int flag1 = 0, index = 0, flag2 = 0;
    Node* rnode = new Node;
    Node* stepnode = new Node;
    (rnode->position).x = rand() % 400 + 1;
    (rnode->position).y = rand() % 400 + 1;
    index = near_node(*rnode);
    if((node_dist(rnode->position, nodes[index]->position)) < step_size)
        return;
    else
        stepnode->position = stepping(nodes[index]->position, rnode->position);
    flag1 = notObstructed(nodes[index]->position.x, nodes[index]->position.y, stepnode->position.x, stepnode->position.y);
    flag2 = notObstructed(nodes[index]->position.x, nodes[index]->position.y, stepnode->position.x, stepnode->position.y);

    if((flag1 == 1) && (flag2 == 1))
    {
        nodes[totnodes++] = stepnode;
        stepnode->parent = nodes[index];
        (nodes[index]->children).push_back(stepnode);
//        line(img, Point((stepnode->position).y, (stepnode->position).x), Point(nodes[index]->position.y, nodes[index]->position.x), Scalar(0, 255, 255), 2, 8);
        connect((stepnode->position).y, (stepnode->position).x, nodes[index]->position.y, nodes[index]->position.x);
//        LINES THAT CONNECTS STEPNODE WITH NODES[INDEX]
        rect(stepnode->position.x, stepnode->position.y);

//        for(int i=stepnode->position.x - 2; i < stepnode->position.x + 2; i++)
//        {
//            for(int j=stepnode->position.y - 2; j < stepnode->position.y + 2; j++)
//            {
//                if((i<0) || (i>400) || (j<0) || (j>400))
//                    continue;
////              MAKE THE NODE 5X5
////                img.at<Vec3b>(i, j)[0] = 0;
////                img.at<Vec3b>(i, j)[1] = 255;
////                img.at<Vec3b>(i, j)[2] = 0;
//
//
//            }
//        }
        if((notObstructed(end_node.position.x, end_node.position.y, stepnode->position.x, stepnode->position.y)) && (node_dist(stepnode->position,end_node.position) < step_size))
        {
            reached = 1;
            nodes[totnodes++] = &end_node;
            end_node.parent = stepnode;
            (nodes[totnodes-1]->children).push_back(&end_node);
            draw_path();
            SDL_RenderPresent(_renderer);
        }
    }
    iter++;
}

int main()
{
//    img = imread("/home/swadha/rrt.png", CV_LOAD_IMAGE_COLOR);
    init();
    while((reached == 0))
        rrt();

//    namedWindow( "window", WINDOW_AUTOSIZE );
//    imshow("window", img);
//    imwrite("output.png", img);
//    waitKey();
    return 0;
}