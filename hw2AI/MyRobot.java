// Feel free to use this java file as a template and extend it to write your solver.
// ---------------------------------------------------------------------------------

import world.Robot;
import world.World;

import java.awt.*;
import java.util.*;

public class MyRobot extends Robot {
    boolean isUncertain;
	
    public static void uncertainAddToOpen(MyRobot robot, Point current, ArrayList<Point> open, ArrayList<Point> closed, String[][] litWorld, World myWorld) {
        int currentX = current.x;
        int currentY = current.y;
        Point up = new Point(currentX - 1, currentY);
        Point down = new Point(currentX + 1, currentY);
        Point right = new Point(currentX, currentY+1);
        Point left = new Point(currentX, currentY-1);
        String bigOh = "O";
        if (current.y > 0) {
            if (bigOh.equals(litWorld[current.x][current.y -1]) && !(closed.contains(left)) && !(open.contains(left))) {
                open.add(0, left);
            }
        }
        if (current.y < (myWorld.numCols()-1)){
            if (bigOh.equals(litWorld[current.x][current.y +1]) && !(closed.contains(right)) && !(open.contains(right))) {
                open.add(0, right);
            }
        }
        if (current.x > 0) {
            if (bigOh.equals(litWorld[current.x -1][current.y]) && !(closed.contains(up)) && !(open.contains(up))) {
                open.add(0, up);
            }
        }
        if (current.x < (myWorld.numRows()-1)) {
            if (bigOh.equals(litWorld[current.x +1][current.y]) && !(closed.contains(down)) && !(open.contains(down))) {
                open.add(0, down);
            }
        }
    }

    
    public static void uncertainMove(MyRobot robot, World myWorld) {
        String[][] litWorld = new String[myWorld.numRows()][myWorld.numCols()]; //new
        litWorld = flash(robot, myWorld);//new
        
        ArrayList<Point> open = new ArrayList<Point>();
        ArrayList<Point> closed = new ArrayList<Point>();
        int distance = 0;
        int minHeuristic = 10001;
        int minDisttoEnd = 10001;
        Point startPos = robot.getPosition();
        uncertainAddToOpen(robot, startPos, open, closed, litWorld, myWorld);
        Point current = new Point(robot.getPosition());
        int tempMin = 0;
        int index = 0;
        ArrayList<String> map = new ArrayList<String>();
        while (!(nextToFinish(myWorld, current))) {
            if (open.size() <= 0) {
                System.out.println("There is no possible path");
                break;
            }
            for (int i = 0; i < open.size(); i++) {
                tempMin = evaluate(open.get(i), myWorld.getStartPos(), myWorld.getEndPos());
                if (tempMin < minHeuristic) {
                    //update min in temp var (keep track of index)
                    minHeuristic = tempMin;
                    minDisttoEnd = manDist(open.get(i), myWorld.getEndPos());
                    index = i;
                }
                else if ((tempMin == minHeuristic) && (manDist(open.get(i), myWorld.getEndPos())) < minDisttoEnd) {
                    index = i;
                    minDisttoEnd = manDist(open.get(i), myWorld.getEndPos());
                    
                }
            }
            //point with min heuristic
            current = open.get(index);
            //add to close
            closed.add(closed.size(), current);
            //System.out.println("(" + current.x + ", " + current.y + ")");
            //remove from open
            open.remove(index);
            //get the new possible points
            uncertainAddToOpen(robot, current, open, closed, litWorld, myWorld);
            //System.out.println("current position is (" + current.x + ", " + current.y + ")");
            minHeuristic = 10001;
            minDisttoEnd = 10001;
        }
        map = certainTravel(open, closed, myWorld);
        robot.navMap(myWorld, map);
    }

    
    public static String[][] flash(MyRobot robot, World myWorld) {
        String[][] litWorld = new String[myWorld.numRows()][myWorld.numCols()];
        for (int i = 0; i < myWorld.numRows(); i++) {
            for (int j = 0; j<myWorld.numCols(); j++ ) {
                Point current = new Point(i, j);
                double currentProb = pingProb(current, robot, myWorld);
                String guess = convertProb(currentProb);
                litWorld[i][j] = guess;
            }
        }
        /*for (int i = 0; i < myWorld.numRows(); i++) {
            //System.out.print("row " + i + ": ");
            for (int j = 0; j<myWorld.numCols(); j++ ) {
                Point current = new Point(i, j);
                double currentProb = pingProb(current, robot, myWorld);
                String guess = convertProb(currentProb);
                System.out.print(litWorld[i][j] + ", ");
                
            }
            System.out.println();
        }*/
        return litWorld;
    }
    
    /* public static void pingEverywhere(MyRobot robot, World myWorld) {
        for (int i = 0; i < myWorld.numRows(); i++) {
            System.out.print("row " + i + ": ");
            for (int j = 0; j<myWorld.numCols(); j++ ) {
                Point current = new Point(i, j);
                double currentProb = pingProb(current, robot, myWorld);
                String guess = convertProb(currentProb);
                System.out.print(guess + ", ");

            }
            System.out.println();
        }
        for (int i = 0; i < myWorld.numRows(); i++) {
            System.out.print("row " + i + ": ");
            for (int j = 0; j<myWorld.numCols(); j++ ) {
                Point current = new Point(i, j);
                double currentProb = pingProb(current, robot, myWorld);
                String guess = convertProb(currentProb);
                System.out.print(currentProb + ", ");
                
            }
            System.out.println();
        }
    } */
    
    public static String convertProb(double prob) { //need to manually do start and finish
        String oh = "O";
        String x = "X";
        //String s = "S";
        //String f = "F";
        if (prob > .5) {
            return oh;
        }
        else {
            return x;
        }
    }
    
    public static double pingProb(Point look, MyRobot robot, World myWorld) {
        
        double pingAmt = 1000;
        double probOh = 0;
        int countX = 0;
        int countOh = 0;
        String oh = "O";
        String x = "X";
        String trial = "";
        for (int i = 0; i < pingAmt; i++) {
        
            trial = robot.pingMap(look);
            if (trial.equals(oh)) {
                //System.out.println("It said O");
                countOh +=1;
            }
            else if (trial.equals(x)) {
                countX += 1;
            }
        
        }
        //System.out.println(countOh);
        probOh = countOh/pingAmt;
        return probOh;
    }
    
    public void navMap(World myWorld, ArrayList<String> map) {
        String left = "L";
        String right = "R";
        String up = "U";
        int currentX = super.getX();
        int currentY = super.getY();
        for (int i = 0; i < map.size(); i ++) {
            if (left.equals(map.get(i))) {
                Point nextMove = new Point(currentX, currentY - 1);
                super.move(nextMove);
            }
            else if (right.equals(map.get(i))) {
                Point nextMove = new Point(currentX, currentY + 1);
                super.move(nextMove);
            }
            else if (up.equals(map.get(i))) {
                Point nextMove = new Point(currentX-1, currentY);
                super.move(nextMove);
            }
            else {
                Point nextMove = new Point(currentX+1, currentY);
                super.move(nextMove);
            }
            currentX = super.getX();
            currentY = super.getY();
        }
    }

    

    public static boolean nextToFinish(World myWorld, Point current) {
        if (manDist(current, myWorld.getEndPos()) == 1) {
            return true;
        }
        else {
            return false;
        }
    }
    
    public static boolean nextToStart(World myWorld, Point current) {
        if (manDist(current, myWorld.getStartPos()) == 1) {
            return true;
        }
        else {
            return false;
        }
    }

    public static int evaluate(Point a, Point startPos, Point endPos ) {
        int fromStart = manDist(a, startPos);
        int fromEnd = manDist(a,endPos);
        return fromStart+fromEnd;
    }

    public static int manDist(Point a, Point b) { // wrote a pure distance method to use in backwards trace utilized in certainTravel
        int xDist = Math.abs(a.x - b.x);
        int yDist = Math.abs(a.y - b.y);
        return xDist + yDist;
    }

    public static void addToOpen(MyRobot robot, Point current, ArrayList<Point> open, ArrayList<Point> closed) {
        int currentX = current.x;
        int currentY = current.y;
        Point up = new Point(currentX - 1, currentY);
        Point down = new Point(currentX + 1, currentY);
        Point right = new Point(currentX, currentY+1);
        Point left = new Point(currentX, currentY-1);
        String bigOh = "O";

        if (bigOh.equals(robot.pingMap(left)) && !(closed.contains(left)) && !(open.contains(left))) {
            open.add(0, left);
        }
        if (bigOh.equals(robot.pingMap(right)) && !(closed.contains(right)) && !(open.contains(right))) {
            open.add(0, right);
        }
        if (bigOh.equals(robot.pingMap(up)) && !(closed.contains(up)) && !(open.contains(up))) {
            open.add(0, up);
        }
        if (bigOh.equals(robot.pingMap(down)) && !(closed.contains(down)) && !(open.contains(down))) {
            open.add(0, down);
        }
        //System.out.println("addToOpen here. current open size = " + open.size());

    }

    public static void certainMove(MyRobot robot, World myWorld) {
        ArrayList<Point> open = new ArrayList<Point>();
        ArrayList<Point> closed = new ArrayList<Point>();
        int distance = 0;
        int minHeuristic = 10001;
        int minDisttoEnd = 10001;
        Point startPos = robot.getPosition();
        addToOpen(robot, startPos, open, closed);
        Point current = new Point(robot.getPosition()); //new
        int tempMin = 0;
        int index = 0;
        ArrayList<String> map = new ArrayList<String>();
        while (!(nextToFinish(myWorld, current))) { //new
            if (open.size() <= 0) {
                System.out.println("There is no possible path");
                break;
            }
            for (int i = 0; i < open.size(); i++) {
                tempMin = evaluate(open.get(i), myWorld.getStartPos(), myWorld.getEndPos());
                if (tempMin < minHeuristic) {
                    //update min in temp var (keep track of index)
                    minHeuristic = tempMin;
                    minDisttoEnd = manDist(open.get(i), myWorld.getEndPos());
                    index = i;     
                }
                else if ((tempMin == minHeuristic) && (manDist(open.get(i), myWorld.getEndPos())) < minDisttoEnd) {
                    index = i;
                    minDisttoEnd = manDist(open.get(i), myWorld.getEndPos());

                }
            }
            //point with min heuristic
            current = open.get(index);
            //add to close
            closed.add(closed.size(), current);
            //System.out.println("(" + current.x + ", " + current.y + ")");
            //remove from open
            open.remove(index);
            //get the new possible points
            addToOpen(robot, current, open, closed);
            //System.out.println("current position is (" + current.x + ", " + current.y + ")");
            minHeuristic = 10001;
            minDisttoEnd = 10001;
        }
        map = certainTravel(open, closed, myWorld);
        robot.navMap(myWorld, map);
    }
    public static ArrayList<String> certainTravel(ArrayList<Point> open, ArrayList<Point> closed, World myWorld) {
        Point startPos = myWorld.getStartPos(); //get start position
        Point current = myWorld.getEndPos(); // get end position
        Point nextMove = new Point();
        int numOptions = 0;
        int count = 0;
        String left = "L";
        String right = "R";
        String up = "U";
        int imptIndex1 = 0;
        int imptIndex2 = 0;
        ArrayList<String> map = new ArrayList<String>(); //eventual route to the end that will be returned

        while (!(nextToStart(myWorld,current)) && count < 100) { //while the current position isn't the start (we should add the initial node to closed immediately)
            numOptions = 0;
            Point option1 = new Point(); //pointers to points
            Point option2 = new Point();

            for (int i = 0; i < closed.size(); i++) { //iterate through closed looking for points 1 away from current node
                if ((manDist(closed.get(i), current) == 1)&& (numOptions == 0) ){///store first point found in option1
                    option1 = closed.get(i);
                    numOptions += 1;
                    closed.remove(i); //hoping this doesn't cause segfaults
                }
                else if ((manDist(closed.get(i), current) == 1)&& (numOptions > 0) ){//store possible second point in option2 (there can be a max of 2 points in closed that are 1 away)
                    option2 = closed.get(i);
                    numOptions += 1;
                    closed.remove(i);
                }
            }
            if (numOptions > 1) { //if there are two options, go with the one closer to start (can explain in person, but it's right)
                if (manDist(option1, startPos) <= manDist(option2, startPos)) {
                    if (option1.x < current.x) { //if the next node is above it, store D
                        map.add(0,"D");
                    }
                    else if (option1.x > current.x) { //if the next node is below it, store U
                        map.add(0,"U");
                    }
                    else if (option1.y < current.y) { //if the next node is left of it, store R
                        map.add(0,"R");
                    }
                    else if (option1.y > current.y) { //if the next node is right it, store L
                        map.add(0,"L");
                    }
                    current = option1; //then update what the current node is for next iteration of while loop
                }
                else { //same reasoning here but if option2 should be selected
                        if (option2.x < current.x) {
                            map.add(0,"D");
                        }
                        else if (option2.x > current.x) {
                            map.add(0,"U");
                        }
                        else if (option2.y < current.y) {
                            map.add(0,"R");
                        }
                        else if (option2.y > current.y) {
                            map.add(0,"L");
                        }
                        current = option2;
                    }
            }
            else {//if (numOptions == 1){ //same reasoning here but if there is no option2
                if (option1.x < current.x) {
                        map.add(0,"D");
                    }
                    else if (option1.x > current.x) {
                        map.add(0,"U");
                    }
                    else if (option1.y < current.y) {
                        map.add(0,"R");
                    }
                    else if (option1.y > current.y) {
                        map.add(0,"L");
                    }
                    current = option1;
            count += 1;
            }
        }
        if (startPos.x < current.x) {
            map.add(0,"D");
        }
        else if (startPos.x > current.x) {
            map.add(0,"U");
        }
        else if (startPos.y < current.y) {
            map.add(0,"R");
        }
        else if (startPos.y > current.y) {
            map.add(0,"L");
        }
        return map; //return an arraylist of strings, pushed on in reverse order
    }
   
    
    @Override
    public void travelToDestination() {
    }
    public void travelToDestination(World myWorld) {

        
        if (isUncertain) {
			// call function to deal with uncertainty
            uncertainMove(this, myWorld);
        }
        else {
			// call function to deal with certainty
            certainMove(this, myWorld);

        }
    }

    @Override
    public void addToWorld(World world) {
        isUncertain = world.getUncertain();
        super.addToWorld(world);
    }
    
    
    public static void main(String[] args) {
        //System.out.println("hello world");
        try {
			World myWorld = new World("TestCases/myInputFile1.txt", false);
			
            MyRobot robot = new MyRobot();
            robot.addToWorld(myWorld);
			myWorld.createGUI(400, 400, 200); // uncomment this and create a GUI; the last parameter is delay in msecs
            System.out.println("World Size: " + myWorld.numRows() + " rows by " + myWorld.numCols() + " columns");
            //System.out.println(open.size());
            //pingEverywhere(robot, myWorld);
			robot.travelToDestination(myWorld);
            //System.out.println(open.size());

        }

        catch (Exception e) {
            e.printStackTrace();
        }
    }
}
