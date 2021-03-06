/**
 *  This is the main file that calls the RRT* algorithm. 
 *  First, the algorithm generates a plan (vector of points) which is a first viable solution.
 *  Next, it calls the RRT* algorithm on the previouslly built plan to optimize it.
 */

#include"RRTstar.h"


//main function
int main()
{
    //define start and end positions
    Point start_pos(25,475);
    Point end_pos(475, 25);
    //define the raduis for RRT* algorithm (Within a radius of r, RRT* will find all neighbour nodes of a new node).
    float rrt_radius = 25;
    //define the radius to check if the last node in the tree is close to the end position
    float end_thresh = 10;
    //instantiate RRTSTAR class
    RRTSTAR* rrtstar = new RRTSTAR(start_pos,end_pos, rrt_radius, end_thresh);

    //set the width and height of the world
    rrtstar->world->setWorldWidth(500.0);
    rrtstar->world->setWorldHeight(500.0);

    // set step size and max iterations. If the values are not set, the default values are max_iter=5000 and step_size=10.0
    rrtstar->setMaxIterations(10000);
    rrtstar->setStepSize(10.0);

    //Create obstacles
    //Obstacle 1
    Point ob1_1(0, 400); //position of the top left point of obstacle 1
    Point ob1_2(350, 350.0); //position of the bottom right point of obstacle 1
    rrtstar->world->addObstacle(ob1_1, ob1_2);//create obstacle 1
    //Obstacle 2;
    Point ob2_1(150, 150.0); //position of the top left point of obstacle 2
    Point ob2_2(500, 100.0); //position of the bottom right point of obstacle 2
    rrtstar->world->addObstacle(ob2_1, ob2_2);//create obstacle 2
    //Save obstacles to  file;
    rrtstar->world->saveObsToFile("Mfiles//Obstacles.txt");

    //clear saved paths from previous run
    rrtstar->savePlanToFile({}, "Mfiles//first_viable_path.txt", {});
    rrtstar->savePlanToFile({}, "Mfiles//Path_after_MAX_ITER.txt", {});


    // RRT* Algorithm
    /*
     Description of RRT* algorithm: 
    1. Pick a random node "N_rand".
    2. Find the closest node "N_Nearest" from explored nodes to branch out towards "N_rand".
    3. Steer from "N_Nearest" towards "N_rand": interpolate if node is too far away. The interpolated Node is "N_new"
    4.  Check if an obstacle is between new node and nearest nod.
    5. Update cost of reaching "N_new" from "N_Nearest", treat it as "cmin". For now, "N_Nearest" acts as the parent node of "N_new".
    6. Find nearest neighbors with a given radius from "N_new", call them "N_near"
    7. In all members of "N_near", check if "N_new" can be reached from a different parent node with cost lower than Cmin, and without colliding
    with the obstacle. Select the node that results in the least cost and update the parent of "N_new".
    8. Add N_new to node list.
    9. Rewire the tree if possible. Search through nodes in "N_near" and see if changing their parent to "N_new" lowers the cost of the path. If so, rewire the tree and
    add them as the children of "N_new" and update the cost of the path.
    10. Continue until maximum number of nodes is reached or goal is hit.
    */

    std::cout << "Starting RRT* Algorithm..." << std::endl;
    //search for the first viable solution
    std::vector<Point> initial_solution =rrtstar->planner();
    
    //save initial solution
    rrtstar->savePlanToFile(initial_solution, "Mfiles//first_viable_path.txt", "First viable solution . This file contains vector of points of the generated path.");
    if (!initial_solution.empty()) {
        std::cout << "First Viable Solution Obtained after " << rrtstar->getCurrentIterations() << " iterations" << std::endl;
        std::cout << "Cost is " << rrtstar->lastnode->cost << std::endl;
        std::cout << "Saving the generated plan (vector of points)" << std::endl;
    }
    std::vector<Point> optimized_solution;
    //search for the optimized paths
    while (rrtstar->getCurrentIterations() < rrtstar->getMaxIterations() && !initial_solution.empty())
    {
        std::cout << "=========================================================================" << std::endl;
        std::cout << "The algorithm continues iterating on the current plan to improve the plan" << std::endl;
        optimized_solution = rrtstar->planner();
        std::cout << "More optimal solution has obtained after " << rrtstar->getCurrentIterations() << " iterations" << std::endl;
        std::cout << "Cost is " << rrtstar->m_cost_bestpath << std::endl;
    }
    //save optimized solution
    rrtstar->savePlanToFile(optimized_solution, "Mfiles//Path_after_MAX_ITER.txt", " Optimized Solution after maximum provided iteration.This file contains vector of points of the generated path.");
    if (!optimized_solution.empty()) {
        std::cout << "Exceeded max iterations!" << std::endl;
        std::cout << "Saving the generated plan (vector of points)" << std::endl;
    }
   
    //free up the memory
    delete rrtstar;
}