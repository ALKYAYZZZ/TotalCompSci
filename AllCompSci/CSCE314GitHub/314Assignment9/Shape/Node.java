import java.util.Iterator;

public final class Node<T extends Shape> implements Iterable<T>{
    public final T v;
    public Node<T> next;
    public Node (T val, Node<T> link) { v = val; next = link; }

    //Create new iterator instance when calling iterator
    @Override
    public Iterator<T> iterator(){
        return new NodeIterator<T>(this);
    }
}