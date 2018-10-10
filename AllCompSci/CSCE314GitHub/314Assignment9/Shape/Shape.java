import java.util.Arrays;

public abstract class Shape implements java.lang.Comparable<Shape>{
    public abstract Point position();
    public abstract double area();
    @Override
	public abstract int compareTo(Shape s);
}