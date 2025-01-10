package RMI.Prove.Prova;

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;

public class UniServiceImpl extends UnicastRemoteObject implements UniService {
    private ArrayList<Registration> r;

    public UniServiceImpl() throws RemoteException {
        super();
        r = new ArrayList<>();
    }

    public String register(String name, String surname, String matriculationNumber, int courseCode) throws RemoteException {
        for (Registration item : r) {
            String fullName = name+surname;
            if (item.find(fullName, courseCode)) {
                System.out.println("You have been already registered");
                return -1;
            }
        }
        String id = randomId();    // Not defined
        Registration r1 = new Registration(name+surname, courseCode, id);
        System.out.println("Student " + name + " " + surname + " registrated with id: " + id);
        return id;
    }

    public void deleteRegistrarion(String id) throws RemoteException {
        boolean found = false;
        for (Registration item : r) {
            if (item.getId() == id) {
                r.remove(item);
                System.out.println("Registration with id: " + id + " deleted");
                found = true;
            }
        }
        if (!found) System.out.println("This id does not exist");

    }

    public int howManyRegistered(int courseCode) throws RemoteException {
        int count = 0;
        for (Registration item : r) {
            if (item.getCourseCode() == courseCode) count++;
        }
        return count;
    }

    public static void main(String[] args) {
        try {
            Naming.rebind("//localhost/UniServiceImpl", new UniServiceImpl());
            System.out.println("Server activated");
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}