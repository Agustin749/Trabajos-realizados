#include <iostream>
#include <conio.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#define VACIO -1
#define TAM 10
#define ARCHCOMPRAS "..."
#define ARCHCLIENTES "..."


using namespace std;

struct Cliente
{
    int UsuarioID = VACIO;
    long FechaCreacion;
    bool Activo = false;
    double TotalImporteCompras = 0;
    char eMail[50];
};

struct Compra
{
    int CompraID = VACIO;
    long long FechaHora;
    double Monto;
    int UsuarioID;
    int NroArticulo;
};

char elegir_menu() //Muestra el menú y devuelve la opcion elegida
{
    int c;
    //system("CLS");
    cout << "Seleccione una opcion:" << endl;
    cout << "a- Cargar un nuevo cliente" << endl;
    cout << "b- Desactivar un cliente existente" << endl;
    cout << "c- Buscar un cliente" << endl;//una vez buscado damos la opccion de ver sus compras o de desactivarlo
    cout << "d- Listar clientes activos ordenados por total del importe" << endl;
    cout << "e- Procesar lote de compra" << endl;
    cout << "f- Mostrar todas las compras realizadas por un cliente (por ID)" << endl;
    cout << "Esc- Finalizar jornada" << endl;

    do{
        c = getch();
    }while(c!=27 &&
            !(c>='A' && c<='F') &&
            !(c>='a' && c<='f')
            ); //bucle para forzar la entrada de una opcion valida
    return c;
}

void cargar_cliente(Cliente &cl) //Pide que el usuario cargue en la variable c un cliente
{
    cout << "\nIngresar ID del cliente (99999):";
    cin >> cl.UsuarioID;
    cout << "\nIngresar Fecha de Creacion (AAAAMMDD):";
    cin >> cl.FechaCreacion;
    cout << "\nIngresar eMail del cliente (pedro@qmail.com.ar):";
    fflush(stdin);
	cin.get(cl.eMail, 50);
    //fflush(stdin);
    cl.Activo = true;
    cl.TotalImporteCompras = 0; //esta en cero ya que supuestamente se acaba de crear el cliente
    return;
 }

bool insertar_cliente(Cliente cl, Cliente vc[TAM]) //Suma el cliente en c al vector de clientes vc
{
    int i;
    for(i=0;i<TAM;i++) //¡¡REVISAR!! No chequea si la información esta duplicada
        if(!vc[i].Activo){ //uso el ID y no el bool activo asi no sobrescribo aquellos desactivados asi se puede seguir accediendo a la info y ademas se van sumando secuencialmente
            vc[i] = cl;
            return true;
        }
    return false;
}

void mostrar_cliente(Cliente cl) //Muestra por pantalla el cliente
{
    int agno = cl.FechaCreacion / 10000;
    int mes = (cl.FechaCreacion % 10000) /100;
    int dia = cl.FechaCreacion % 100;

    cout << "\n************* CLIENTE ***************:" << endl;
    cout << "UsuarioID: " << cl.UsuarioID << endl;
    cout << "eMail: " << cl.eMail << endl;
    cout << "Fecha de Creacion: " << dia << "/" << mes << "/" << agno << endl;
    cout << "Activo: " << cl.Activo << endl;
    cout << "Total de Importe de Compras: $" << cl.TotalImporteCompras << endl;
    return;
}

void mostrar_vclientes(Cliente vc[TAM]) //Muestra el vector de clientes
{
    int i=0;
    for(i;i<TAM;i++)
    {
        if(vc[i].Activo){
            mostrar_cliente(vc[i]);
        }
        else{ // Puedo hacer esto ya que con el bubble se que los clientes desactivados estan ultimos
            cout << "\n**** No hay mas clientes activos ****\n" << endl;
            break;
        }
    }
    return;
}

int busqueda_cliente(Cliente vc[TAM]) //Pide el ID o el Email del cliente, lo busca y devuelve su posicion en el vector (si no lo encuentra devuelve VACIO)
{
    char opcion;
    int i=0;
    int ID=0;
    char eMail[50];
    Cliente cl;

    cout << "a- Buscar por UsuarioID" << endl;
    cout << "b- Buscar por eMail" << endl;
    cout << "Esc- Salir de busqueda" << endl;

    do{
        opcion = getch();
    }while(opcion!=27 &&
            !(opcion=='A' || opcion=='a') &&
            !(opcion=='B' || opcion=='b')
           );

    switch(opcion)
    {
    case 'A':case 'a': //Busqueda por ID
        cout << "\nIngrese el UsuarioID del cliente (99999): ";
        cin >> ID;
        while(i<TAM && vc[i].UsuarioID != ID && vc[i].UsuarioID != VACIO)
            i++;
        if(i<TAM && vc[i].UsuarioID != VACIO)
            return i;
        else
            cout << "\nERROR: No se ha encontrado el Cliente con ID: " << ID << endl;
        break;

    case 'B':case 'b': //Busqueda por eMail
        cout << "\nIngrese el eMail del cliente (pepe@qmail.com): ";
        cin.get(eMail, 50);
        while(i<TAM && strcmp(vc[i].eMail, eMail) != 0 && vc[i].UsuarioID != VACIO) //¿Revisado? (No)
            i++;
        if(i<TAM && vc[i].UsuarioID != VACIO)
            return i;
        else
            cout << "\nERROR: No se ha encontrado el Cliente con eMail: " << eMail << endl;
        break;

    case 27:
        cout << "\nSe cancelo la busqueda" << endl;
        break;
    }
    return VACIO;
}

void desactivar_cliente(Cliente vc[TAM], int pos) //Desactiva el cliente de la posicion que se le pase, mostrándolo antes para evitarle errores al ususario
{
    if(pos != VACIO) //La busqueda devuelve VACIO si se cancela o si no se encuentra el cliente
    {
        cout << "\nSe desactivara el siguiente cliente: \n" << endl;
        mostrar_cliente(vc[pos]);
        cout << "Presione una tecla para continuar (o Esc para cancelar)" << endl;

        if(getch()!=27){
            vc[pos].Activo = false;
            cout << "Cliente desactivado correctamente" << endl;
        }
    }
    else
        cout << "No se ha desactivado el Cliente" << endl;
    return;
}

void bubble_ximporte(Cliente vc[TAM]) //Ordena los clientes activos con mayor importe primero y todos aquelos desactivados quedan ultimos
{
    int i=0,j=0;
    Cliente aux;

    for (i;i<TAM;i++)
    {
        if(vc[i].UsuarioID == VACIO) // Si se llega al final lógico del vector se termino con el bubble
                break;

        for(j=i+1;j<TAM;j++)
        {
            if(vc[j].UsuarioID == VACIO) // Si se llega al final lógico del vector se sale de este loop
                break;
            if((vc[i].TotalImporteCompras<vc[j].TotalImporteCompras && vc[j].Activo) ||
               (!(vc[i].Activo) && vc[j].Activo)) //Los cambia de lugar si el primero es menor al otro o si el primero esta desactivado
            {
                aux = vc[i];
                vc[i] = vc[j];
                vc[j] = aux;
            }
        }

    }
    return;
}

void cargar_lote(Compra &com) // Carga la info al lote que se le pase
{
    long long fecha,hora;
    cout << "Ingresar el ID de la compra (9999): ";
    cin >> com.CompraID;
    cout << "Ingresar Fecha de la compra (AAAAMMDD): ";
    cin >> fecha;
    cout << "Ingresar Hora de la compra (HHMM): ";
    cin >> hora;
    cout << "Ingresar el monto de la compra (999.999999): ";
    cin >> com.Monto;
    cout << "Ingresar el ID del cliente (99999): ";
    cin >> com.UsuarioID;
    cout << "Ingresar Nro del articulo (99999): ";
    cin >> com.NroArticulo;

    com.FechaHora = fecha*10000 + hora;
    return;
}

bool sumar_importe(Compra &com, Cliente vc[TAM])//Actualiza el importe del cliente que hizo la compra y si este no existe, devuelve false
{
    int i=0;
    while(i<TAM && vc[i].UsuarioID != com.UsuarioID && vc[i].UsuarioID != VACIO)
            i++;
        if(i<TAM && vc[i].UsuarioID != VACIO)
            vc[i].TotalImporteCompras += com.Monto;
        else{
            cout << "\nERROR: No se ha encontrado el Cliente con ID:" << com.UsuarioID << endl;
            return false; //no se encontro el cliente
        }
    return true;
}

int conteoCompras(Compra &lote){
	FILE *p2;
    
    p2 = fopen("procesados.bin", "ab+");
    fseek(p2,0,SEEK_END);
    int file_length = ftell(p2);
    int nRegistros = file_length/sizeof(struct Compra);
    fclose(p2);
    return nRegistros;
}

void insertar_lote(Compra &lote) // Inserta el lote al archivo
{
	FILE *p2;
    
    p2 = fopen("procesados.bin", "ab+");
	int nRegistros = conteoCompras(lote);
    fseek(p2, sizeof(struct Compra)*nRegistros, SEEK_SET);
    fwrite(&lote, sizeof(struct Compra), 1, p2);
	fclose(p2);
	
    return;
}

void clientespruebas(Cliente vcliente[TAM]) // Carga un par de clientes (PARA HACER PRUEBAS)
{
	vcliente[0].UsuarioID = 12345;
    strcpy(vcliente[0].eMail, "bruno_olaso@hotmail.com");
	vcliente[0].TotalImporteCompras = 230.545678;
    vcliente[0].FechaCreacion = 20201128;
    vcliente[0].Activo = true;

    vcliente[1].UsuarioID = 23456;
    strcpy(vcliente[1].eMail, "brunoolasoalaya@gmail.com");
    vcliente[1].TotalImporteCompras = 1432.25;
    vcliente[1].FechaCreacion = 20121128;
    vcliente[1].Activo = true;

    vcliente[2].UsuarioID = 98765;
    strcpy(vcliente[2].eMail, "bolasoalaya@est.frba.utn.edu.ar");
    vcliente[2].TotalImporteCompras = 3.25;
    vcliente[2].FechaCreacion = 20101101;
    vcliente[2].Activo = true;
    
    /*FILE *p1;
    
    p1 = fopen("Clientes.bin", "wb");
    fwrite(vcliente, sizeof(struct Cliente), TAM, p1);
	fclose(p1);*/
	
	/*Cliente vcliente2[TAM];
    
    p1 = fopen("Clientes.bin", "rb");
    fread(vcliente2, sizeof(struct Cliente), 3, p1);
    for(int i=0; i<3 ; i++){
    	cout << vcliente2[i].eMail << endl;
	}
	fclose(p1);*/
    
	return;
    

}

void importarClientes(Cliente vcliente[TAM])
{

	FILE *p1;
	
	p1 = fopen("Clientes.bin", "rb");
	fread(vcliente, sizeof(struct Cliente), TAM, p1);
	fclose(p1);
}

int cantClientesActivos(Cliente vcliente[TAM])
{
	int i=0;
	while(vcliente[i].Activo){
		i++;
	}
	return i;
}

void exportarClientes(Cliente vcliente[TAM])
{
	FILE *p1;
	
	p1 = fopen("Clientes.bin", "wb");
	fwrite(vcliente, sizeof(struct Cliente), cantClientesActivos(vcliente), p1);
	fclose(p1);
}

void mostrarComprasPorID(int totalCompras){
	int usuarioIDTemp;	
	bool check = false;
	cout << "Ingrese el ID del usuario: ";
	cin >> usuarioIDTemp;        
	FILE *p2;
	p2 = fopen("procesados.bin", "rb");
	Compra compraTemp;
	for(int i=1; i<=totalCompras; i++)
	{
		fread(&compraTemp, sizeof(struct Compra), 1, p2);
			if(usuarioIDTemp == compraTemp.UsuarioID)
			{
				cout << "--------------------------------------" << endl;
				cout << "Compra ID: " << compraTemp.CompraID << endl;
				long long fecha = (compraTemp.FechaHora / 10000);
				long long hora = compraTemp.FechaHora - (compraTemp.FechaHora / 10000)*10000;
				cout << "Fecha y Hora: " << fecha - (fecha/100)*100 << "/" << fecha/100 - (fecha/10000)*100 << "/" << fecha/10000;
				cout <<  " (" << hora/100 << ":" << hora - (hora/100)*100 << ")" << endl;
				cout << "Monto: $" << compraTemp.Monto << endl;
				cout << "Usuario ID: " << compraTemp.UsuarioID << endl;
				cout << "Numero de articulo: " << compraTemp.NroArticulo << endl;
				check = true;
			}
	}
	if (!check) cout << "No encontramos ninguna compra con ese ID" << endl;
	
	fclose(p2);
}


int main()
{
	int totalCompras = 0;
    int i;
    char opcion;
    Cliente vcliente[TAM];
    //Cliente vclienteaux[TAM];
    //clientespruebas(vclienteaux);
    Cliente cliente;//"buffer" para el vector
    Compra lote; //buffer
    int pos; // posicion de un cliente en el vector
	cout << setprecision(5) << fixed;
	importarClientes(vcliente);
	cout << cantClientesActivos(vcliente) << endl;
    do{
        opcion = elegir_menu();
        fflush(stdin);
        switch(opcion)
        {
        case 'A':case 'a': //Carga de cliente
            cout << "\nHa seleccionado:\na- Cargar un nuevo cliente" << endl;
            cout << "\n---------------- CARGA DE CLIENTE -----------------" << endl;

            cargar_cliente(cliente);//carga los datos a la variable del argumento
            if (insertar_cliente(cliente, vcliente))
                cout << "\n-- Cliente ingresado correctamente --" << endl;
            else cout << "\nERROR: No se pueden ingresar mas clientes, desactive un cliente existente" << endl;
            break;

        case 'B':case 'b': //DesactiVACIOn de cliente
            cout << "\nHa seleccionado:\nb- Desactivar un cliente existente " << endl;
            cout << "\n-------- BUSQUEDA DE CLIENTE A DESACTIVAR ---------" << endl;

            pos = busqueda_cliente(vcliente); //posicion del cliente en el vector
            desactivar_cliente(vcliente, pos);
            break;

        case 'C':case 'c': //Busqueda de cliente
            cout << "\nHa seleccionado:\nc- Buscar un cliente" << endl;
            cout << "\n------------- BUSQUEDA DE CLIENTE -----------------" << endl;

            pos = busqueda_cliente(vcliente);
            if(pos != VACIO)mostrar_cliente(vcliente[pos]);
            break;

        case 'D': case 'd':
            cout << "\nHa seleccionado:\nd- Listar clientes activos ordenados por total del importe" << endl;
            cout << "\n----------- LISTADO CLIENTES ACTIVOS -------------" << endl;

            bubble_ximporte(vcliente);
            mostrar_vclientes(vcliente);
            break;

        case 'E': case 'e':
            cout << "\nHa seleccionado:\ne- Procesar lote de compra" << endl;
            cout << "\n-------- PROCESAMIENTO DE LOTE DE COMPRA ---------" << endl;

            cargar_lote(lote);
            if(sumar_importe(lote,vcliente))// Esto sirve para no agregar lotes al archivo de clientes que no existen
                insertar_lote(lote);
			else
                cout << "\nNo se agrego el lote de compra" << endl;
    		break;
        
        case 'F': case 'f':
            cout << "\nHa seleccionado:\nf- Mostrar todas las compras realizadas por un cliente (por ID)" << endl;
            cout << "\n-------- COMPRAS REALIZADAS DEL CLIENTE ---------" << endl;
            
            totalCompras = conteoCompras(lote);
            
            mostrarComprasPorID(totalCompras);
            break;
        }

	

        if (opcion == 27) break; // asi evito la linea de abajo

        cout << "\nPresionar una tecla para volver al menu" << endl;
        getch();
        system("cls");

	
    } while(true);
    
    exportarClientes(vcliente);

    return 0;
}
