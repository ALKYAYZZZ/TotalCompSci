import java.util.Iterator;
import java.util.*;

public class ShapeList<T extends Shape> implements Iterable<T>{
    public ArrayList<T> list;
    //Initialization is an empty arraylist
    ShapeList(){
        ArrayList<T> t = new ArrayList<T>();
        this.list = t;
    }

    //Constructor adds all shapes to arraylist
    ShapeList(Iterable<T> iterable){
        ArrayList<T> t = new ArrayList<T>();
        for(T temp: iterable){
            t.add(temp);
        }
        this.list = t;
    }

    //Use reverse function to reverse ArrayList
    public ShapeList<T> reverse(){
        Collections.reverse(this.list);
        return this;
    }

    //Print out all shapes using iterator
    public String toString(){
        System.out.print("[");
        if(this.list.isEmpty() == false){
            for(T t: this.list){
                t.toString();
                System.out.print(", ");
            }
        }
        System.out.print("]\n");
        

        return "";
    }

    @Override
    public Iterator<T> iterator(){
        return this.list.iterator();
    }
}