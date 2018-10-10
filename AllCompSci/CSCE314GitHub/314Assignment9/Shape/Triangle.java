public class Triangle extends Shape{
    public Point pointA;
    public Point pointB;
    public Point pointC;
    
    public Triangle(Point a, Point b, Point c){
        pointA = a;
        pointB = b;
        pointC = c;
    }

    public Point position(){
        double xtotal = (pointA.getX() + pointB.getX() + pointC.getX())/3;
        double ytotal = (pointA.getY() + pointB.getY() + pointC.getY())/3;
        Point newpoint = new Point(xtotal, ytotal);
        return newpoint;
    }

    public double area(){
        double result = Math.abs((pointA.getX()*(pointB.getY() - pointC.getY())+ pointB.getX()*(pointC.getY() - pointA.getY()) + pointC.getX()*(pointA.getY() - pointB.getY()))/2);
        System.out.println(result);
        return result;
    }

    //See if triangles are congruent
    public boolean equals(Shape s){
        boolean result = false;
        if(s instanceof Triangle){
            if(this.position() == s.position()){
                if(this.area() == s.area()){
                    result = true;
                }
            }
        }
        return result;
    }

    //Override compare functions for iterator
    @Override
    public int compareTo(Shape s){
        if(this.area() > s.area()){
            return 1;
        }
        else if(this.area() < s.area()){
            return -1;
        }
        else{
            return 0;
        }
    }

    public String toString(){
        System.out.print("Triangle (" + pointA.getX() + "," + pointA.getY() + ")-(" + pointB.getX() + "," + pointB.getY() + ")-(" + pointC.getX() + "," + pointC.getY() + ")");
        return "";
    }
}