#include <iostream>
#include <array>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
using Cubo=array<array<array<int,12>,3>,4>;	
void mostrar(Cubo&); 
void cargar(Cubo&); 
int mayor(Cubo&,int,int); 
void Mejorvendedor(const Cubo&,int,int); 
int GetPromedio(const Cubo&,int,int); 
void estadisticas (Cubo&);
int main()
{	
Cubo total{};	
cargar(total);
mostrar(total);
estadisticas(total);	
}
void mostrar(Cubo&total)
{
ofstream file;
file.open("file.txt",ios::out|ios::binary);
for (int i{};i<4;++i)
 { 
 	cout<<'\n'<<'\t'<<"LA REGION: "<<i+1<<'\n';
 	for (int j{};j<3;++j)
 	{
 		cout<<'\n'<<"El vendedor: "<<j+1<<'\n';
 		for (int k{};k<12;++k)
 		{
          cout<<"En el mes: "<<k+1<<" vendio: "<<total[i][j][k]<<'\n';
 		}
 	}
 }	
 file.write(reinterpret_cast<char*>(&total),sizeof(total));
 file.close();
}
void cargar(Cubo&total)
{
 ifstream file2;
 file2.open("file.txt",ios::in|ios::binary);
 file2.read(reinterpret_cast<char*>(&total),sizeof(total));
 int v{},m{},r{},importe{};
 for(;cin>>r>>v>>m>>importe;)
 {
	total.at(r-1).at(v-1).at(m-1)+=importe;
 }
 file2.close();
}
int mayorVenta(Cubo&total,int mes,int region)
{
  int max{};
 	for (auto vendedoraux:total.at(region))
    	if(vendedoraux.at(mes)>max) 
    		max=vendedoraux.at(mes);
 return max;
}
void Mejorvendedor (const Cubo&total,int mes,int region)
{
 int posicion{},max{};
 array<int,3> vectoraux{};
 bool repetido=false;
 	for (int j{};j<3;j++)
	{
		if(total[region][j][mes]==max && repetido!=true)
		{
			repetido=true;
		}
		if(total[region][j][mes]>max)
		{
		    max=total[region][j][mes];
    		posicion=j+1;
		}	
	}
	for(int k{0};k<3;k++)
	{ 
		if(total[region][k][mes]==max)
		{
		 vectoraux.at(k)=k+1;
		}
		else vectoraux.at(k)=0;
	}
	if(repetido==true)
	{
	cout<<"Los mejores vendedores del mes fueron: ";
	for(int l{};l<3;l++)
	{
		if(vectoraux.at(l)!=0)
		cout<<vectoraux.at(l)<<" ";
	}
	}
	else	
        cout<<"El mejor vendedor del mes fue: "<<posicion;
}
int GetPromedio(const Cubo&total,int mes,int region)
{
	int s{};
	for (auto meses:total.at(region))
			s += meses.at(mes);
	return s/total.at(region).size(); 
}
void estadisticas (Cubo&total)
{
for (int i{};i<12;++i)
{
  for (int j{};j<4;++j)
  {
    cout<<'\n'<<"El importe maximo en el mes "<<i+1<<" region "<<j+1<<" es: "<<mayorVenta(total,i,j)<<'\n';
	Mejorvendedor(total,i,j);
    cout<<'\n'<<"El importe promedio en el mes "<<i+1<<" region "<<j+1<<" es: "<<GetPromedio(total,i,j)<<'\n';
  }
}
}
