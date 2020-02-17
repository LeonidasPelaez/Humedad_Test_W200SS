
int   Voltaje_Lectura;

int   Volt_Sumatoria_Directa;
float   Volt_Promedio_Directa;
float   Resistencia_Directa;

int   Volt_Sumatoria_Inversa;
float   Volt_Promedio_Inversa;
float   Resistencia_Inversa;
       
float   Res_Sensor;

int   i;

int   R1 = 10000;

float kPa;


#define D10_PIN_DIGITAL   10 
#define D11_PIN_DIGITAL   11   
#define A0_PIN_ANALOGICO  A0  

void setup( ) 
{
    pinMode(D10_PIN_DIGITAL, OUTPUT); // configuracion como salida
    pinMode(D11_PIN_DIGITAL, OUTPUT); // configuracion como salida

    digitalWrite(D10_PIN_DIGITAL, LOW); // pin en nivel bajo
    digitalWrite(D11_PIN_DIGITAL, LOW);
}

    
void loop()
{  
    digitalWrite(D10_PIN_DIGITAL, HIGH); // pin en nivel alto
    digitalWrite(D11_PIN_DIGITAL, LOW);  // pin en nivel bajo
    delay(1200);
    Volt_Sumatoria_Directa = 0;
    
    for( i=0; i < 10; i++ )  // se hacen 10 lecturas cada 100 ms
       {
           Voltaje_Lectura = analogRead(A0_PIN_ANALOGICO);
           Volt_Sumatoria_Directa = Volt_Sumatoria_Directa + Voltaje_Lectura; // se va acumulando todas las lecturas 
           delay(100);        
       }
    Volt_Promedio_Directa = Volt_Sumatoria_Directa/10;  // se saca promedio
    Volt_Promedio_Directa = (Volt_Promedio_Directa * (5.0 / 1024.0)); // se hace la conversion de binario a voltios
    Resistencia_Directa = Volt_Promedio_Directa*R1/(5.0 - Volt_Promedio_Directa); // se obtiene la resistencia

    //==================================================================================
    // se realiza la polarizacion inversa para obtener lecturas mas veridicas 
    digitalWrite(D10_PIN_DIGITAL, LOW);
    digitalWrite(D11_PIN_DIGITAL, HIGH);
    delay(1200); // se espera para que se descargue capacitancias parasitas
    Volt_Sumatoria_Inversa = 0;
    
    for( i=0; i < 10; i++ )
       {
           Voltaje_Lectura = analogRead(A0_PIN_ANALOGICO);
           Volt_Sumatoria_Inversa = Volt_Sumatoria_Inversa + Voltaje_Lectura;  
           delay(100);        
       }
    Volt_Promedio_Inversa = Volt_Sumatoria_Inversa/10;
    Volt_Promedio_Inversa = (Volt_Promedio_Inversa * (5.0 / 1024.0));    
    Resistencia_Inversa = R1*( (5.0/Volt_Promedio_Inversa) - 1 );
    //==================================================================================
       
    Res_Sensor = (Resistencia_Directa + Resistencia_Inversa)/2;  // se promedia las resistencias en ambos tipos de polarizaciones

    //kPa= (-3.213*R - 4.093) / (1 - 0.009733*R - 0.01205*T)
    kPa = (-3.213*Res_Sensor - 4.093) / (1 - 0.009733*Res_Sensor - 0.01205*24); // se hace la conversion a CBA o KPA

    
}
