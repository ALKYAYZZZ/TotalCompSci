import java.util.Scanner;
import java.util.Arrays;

public abstract class Main extends Shape{


public static void main(String[] args){
    Shape shape[] = new Shape[args.length];
    
    int counter = 0;
    for(String arguments: args){
        String no = arguments;
        Scanner scan = new Scanner(arguments);
        char help = scan.next().charAt(0);
        System.out.println(no);
        if(help == 'r'){
            double x1 = scan.nextDouble();
            double y1 = scan.nextDouble();
            double x2 = scan.nextDouble();
            double y2 = scan.nextDouble();
            Point point1 = new Point(x1, y1);
            Point point2 = new Point(x2, y2);
            Rectangle rec = new Rectangle(point1, point2);
            shape[counter] = rec;
        }
        else if(help == 'c'){
            double x1 = scan.nextDouble();
            double y1 = scan.nextDouble();
            double r = scan.nextDouble();
            Point point1 = new Point(x1, y1);
            Circle circle = new Circle(point1, r);
            shape[counter] = circle;
        }
        else if(help == 't'){
            double x1 = scan.nextDouble();
            double y1 = scan.nextDouble();
            double x2 = scan.nextDouble();
            double y2 = scan.nextDouble();
            double x3 = scan.nextDouble();
            double y3 = scan.nextDouble();
            Point point1 = new Point(x1, y1);
            Point point2 = new Point(x2, y2);
            Point point3 = new Point(x3, y3);
            Triangle tri = new Triangle(point1, point2, point3);
            shape[counter] = tri;
        }
        counter++;
    }
    System.out.printf("The total area for the %d objects is %1.2f units squared.\n", shape.length, AreaCalculator.calculate(shape)); 
    Arrays.sort(shape);
    int count = 0;
    for (Shape s: shape) {
        System.out.println(s.toString());
        System.out.println(++count + ") "+s+"\t\t area="+s.area());
    }
    Node<Shape> temp1 = new Node<Shape>(shape[1], null);
    Node<Shape> temp = new Node<Shape>(shape[0], temp1);
    Shape max = maxArea(temp);
    System.out.println("Max Area is: " + max.area());

    /*
    Node<Rectangle> temp4 = new Node<Rectangle>(shape[1], null);
    Node<Rectangle> temp3 = new Node<Rectangle>(shape[0], temp1);
    Rectangle rect = boundingRect(temp3);
    System.out.println("Bounded Rect: " + rect.toString());
    */
    Point p1 = new Point(1,1);
    Point p2 = new Point(2,2);
    Point p3 = new Point(3,3);
    Circle c1 = new Circle(p1,1);
    Circle d1 = new Circle(p2,2);
    Circle e1 = new Circle(p3,3);

    ShapeList<Shape> emptyShapes = new ShapeList<Shape>();
    ShapeList<Circle> someCircles = new ShapeList<Circle>(Arrays.asList(c1, d1, e1));

    System.out.println("emptyShapes = " + emptyShapes);
    System.out.println("reversed emptyShapes = " + emptyShapes.reverse());
    System.out.println("someCircles = " + someCircles);
    System.out.println("reversed someCircles = " + someCircles.reverse());

    double sumOfXs = 0.0;
    double sumOfYs = 0.0;
    for (Circle c: someCircles) {
        sumOfXs += c.position().getX();
        sumOfYs += c.position().getY();
    }
    System.out.println("Some of Xs = " + sumOfXs);
    System.out.println("Some of Ys = " + sumOfYs);

    }

    //Go through nodes using iterator to find greatest area
    public static Shape maxArea(Node<Shape> list){
        Shape biggest = list.v;
        for(Shape s: list){
            if(s.area() > biggest.area()){
                biggest = s;
            }
        }
        return biggest;
    }

    //Find the greatest X and Y value and smallest X and Y value and make new rectangle
    public static Rectangle boundingRect(Node<Rectangle> list){
            Rectangle large = list.v;
            double upperX = 0;
            double upperY = 0;
            double lowerX = 0;
            double lowerY = 0;
            for(Rectangle r: list){
                if(r.point1.getX() > upperX){
                    upperX = r.point1.getX();
                }
                if(r.point1.getY() > upperY){
                    upperY = r.point1.getX();
                }
                if(r.point1.getX() < lowerX){
                    lowerX = r.point1.getX();
                }
                if(r.point1.getX() < lowerY){
                    lowerY = r.point1.getY();
                }
                if(r.point2.getX() > upperX){
                    upperX = r.point2.getX();
                }
                if(r.point2.getY() > upperY){
                    upperY = r.point2.getX();
                }
                if(r.point2.getX() < lowerX){
                    lowerX = r.point2.getX();
                }
                if(r.point2.getX() < lowerY){
                    lowerY = r.point2.getY();
                }
            }
            Point upper = new Point(upperX, upperY);
            Point lower = new Point(lowerX, lowerY);
            Rectangle rect = new Rectangle(upper, lower);
            return rect;
    }
}