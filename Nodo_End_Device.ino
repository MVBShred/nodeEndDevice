#include <DHT.h>

#define DHTPIN 2

#define DHTTYPE DHT22 // Tipo de Sensor; en este caso DHT22

#include <XBee.h>
 
uint8_t payload[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

String result = "";
String hexstring = "";
 
XBee xbee = XBee();   

XBeeAddress64 addr64 = XBeeAddress64(0x00, 0xffff);

unsigned long contador = 0; // unsigned long 0 to 4,294,967,295; long  from -2,147,483,648 to 2,147,483,647

DHT dht(DHTPIN, DHTTYPE);


// union to convery float to byte string
union u_tagF
{
    uint8_t bF[4];
    float fval;
} f;

// union to convery usigned long to byte string
union u_tagUl
{
    uint8_t bUl[4];
    unsigned long ulval;
} ul;

// union to convery int to byte string
union u_taginI
{
    uint8_t bI[2];
    int ival;
} in;

void setup() {

  // put your setup code here, to run once:
    Serial.begin(9600);

  // Se inicia el dht
    dht.begin();

}

void loop() {
  
    float h = dht.readHumidity();    // Leemos la Humedad
    float t = dht.readTemperature(); // Leemos la temperatura en grados 
    
// Cambiar dependiendo del nodo a usar.
   int id = 2;

      // Id del dispositivo
    in.ival = id;
    for (int i = 0; i < 2; i++)
    {
        payload[i] = in.bI[i];
    }

    // Envío del contador
    ul.ulval = contador;
    for (int i = 0; i < 4; i++)
    {
        payload[i + 2] = ul.bUl[i];
    }

    // DHT22 humedad
    f.fval = h;
    for (int i = 0; i < 4; i++)
    {
        payload[ i + 6 ] = f.bF[i];
    }

    // DHT22 temperatura
    f.fval = t;
    for (int i = 0; i < 4; i++)
    {
        payload[i + 10] = f.bF[i];
    }

  //----------------------------------------------------------------------------------------------------------------------

   for(int i = 0; i < 14; i++) {
    if(payload[i] < 0x10) {
      hexstring += '0';
    }

    hexstring += String(payload[i], HEX) +" ";
  }

  result = HexString2ASCIIString(hexstring);

  Serial.println("Resultado =" +result);
  Serial.println();


  // put your main code here, to run repeatedly:

  ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
  
  
  xbee.send(zbTx);
  Serial.println();

  // Aumentamos el contador
    contador++;
    delay(5000);

}



String HexString2ASCIIString(String hexstring) {
  String temp = "", sub = "", result;
  char buf[3];
  for(int i = 0; i < hexstring.length(); i += 2){
    sub = hexstring.substring(i, i+2);
    sub.toCharArray(buf, 3);
    char b = (char)strtol(buf, 0, 16);
    if(b == '\0')
      break;
    temp += b;
  }
  return temp;
}


