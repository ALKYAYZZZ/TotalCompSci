import java.lang.Class;
import java.lang.reflect.*;
import static java.lang.System.err;
import java.util.*;
import java.io.*;

public class Main3{
	public static void main(String[] args){
		A a = new A();
		displayMethodInfo(a);
	}
	
	static void displayMethodInfo(Object o){
		Class s = o.getClass();
		Method m[] = s.getDeclaredMethods();
	
	//Use reflection to find all classes of object and display
	try{
		for(int i = 0; i < m.length; i++){
			System.out.print(m[i].getName() + "(");
			if((Modifier.isStatic(m[i].getModifiers())) == false){
				String q = m[i].getDeclaringClass().toString();
				String[] qList = q.split(" ");
				System.out.print(qList[1]);
			}
			
			Type[] tList = m[i].getGenericParameterTypes();
			for(Type j: tList){
				System.out.print(" ," + j.toString());
			}
			System.out.println(") ->" + m[i].getGenericReturnType().toString());
		}
		
	}
	catch(Throwable t){
	System.err.println(t);	
	}
	}
}