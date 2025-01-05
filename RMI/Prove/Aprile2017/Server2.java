package RMI.Prove.Aprile2017;

import java.rmi.RemoteException;
// import java.rmi.registry.LocateRegistry;
// import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

public class Server2 extends UnicastRemoteObject implements RemoteService2 {
    public Server2() throws RemoteException {
        super();
    }

    @Override
    public int[] encontrarMinimo(int[] vector) throws RemoteException {
        int minimo = Integer.MAX_VALUE;
        int posicion = -1;

        for (int i = 0; i < vector.length; i++) {
            if (vector[i] < minimo) {
                minimo = vector[i];
                posicion = i;
            }
        }
        return new int[]{minimo, posicion};
    }

    public static void main(String[] args) {
        try {
            Server2 server2 = new Server2();
            System.out.println("HELLO");
            // Registry registry = LocateRegistry.getRegistry();
            // registry.rebind("Server2", server2);
            java.rmi.Naming.rebind("//localhost/Server2", server2);
            System.out.println("Server2 listo.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
