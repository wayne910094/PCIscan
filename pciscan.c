#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <conio.h>
#include <stdio.h>



extern EFI_SYSTEM_TABLE          *gST;

#define SCAN_NULL       0x0000
#define SCAN_UP         0x0001
#define SCAN_DOWN       0x0002
#define SCAN_ESC        0x0017
#define CHAR_CARRIAGE_RETURN  0x000D

#define	LINEX 3
#define	LINEY 3
#define NUM  5


GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mStringHelpTokenId = STRING_TOKEN (STR_HELLO_WORLD_HELP_INFORMATION);

long pcideviceval(long Bus,long Dev,long Fun)
{             
  long reg,val,Address;
  long val1,val2,val3,val4;
  int i=0;
  
  for(reg=0;reg<256;reg=reg + 4){
    Address = 0x80000000|(Bus<<16)|(Dev<<11)|(Fun<<8)|reg; 
    outpd(0xcf8,Address);            
    val = inpd(0xcfc);               
    if(reg==0){
      if(val == 0xffffffff){
        break;
        }
      }
    
    val4 = val>>24 & 0xff;
    val3 = val>>16 & 0xff;
    val2 = val>>8 & 0xff;
    val1 = val<<0 & 0xff;

    
    if(i%4==0){
      Print(L"\n");
    }
    Print(L"%02x %02x %02x %02x ",val1,val2,val3,val4);
    i++;

  }return val;                
}



long pcideviceinfo(int i,long *Busc,long *Devc,long *Func){//long *Busc,long *Devc,long *Func
  long Bus,Dev,Fun,reg,val,Address;
  int i=0;
  int j=0;
  for(Bus=0;Bus<256;Bus++){//256個BUS
      for(Dev=0;Dev<32;Dev++){//32個DEVICE
        for(Fun=0;Fun<8;Fun++){//8個FUNCTION
          for(reg=0;reg<256;reg+=4){
            Address = 0x80000000|(Bus<<16)|(Dev<<11)|(Fun<<8)|reg; 
            outpd(0xcf8,Address);
            val = inpd(0xcfc);
            if(reg==0){
              if(val == 0xffffffff){
                break;
                }
              }
            if(i%64==0){
              Bus = Busc[j];
              Dev = Devc[j];
              Fun = Func[j];
              Print(L"%02x %02x %02x \n",Bus,Dev,Fun);
              }
            j++;        
            i++;
            } 

          }
        }
    }
    return 0;
}


EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  long Bus,Dev,Fun,reg,val,Address;
  int i=0;
  int j=0;
  int k=0;
  int l=0;
 
  EFI_INPUT_KEY	Key;
  EFI_STATUS Status;
  int current=0; 
  gST -> ConOut ->ClearScreen(gST->ConOut); 
  gST -> ConOut -> SetCursorPosition(gST -> ConOut,3,3);
  
  for(i=0;i++;){
    pcideviceinfo(Bus[i],Dev[i],Fun[i]);
  }
        
  Key.ScanCode=SCAN_NULL;
  //SCAN_ESC!=Key.ScanCode
 while (1)
    {
		Status= gST -> ConIn -> ReadKeyStroke(gST->ConIn,&Key);	
		if (Status == EFI_SUCCESS)	{
      gST -> ConOut -> SetCursorPosition(gST -> ConOut,LINEX,LINEX+current);      
      Print(L"%S",ITEM[current]);
			if (SCAN_UP == Key.ScanCode) {
        current = (current-1+i)%i;
        }
			if (SCAN_DOWN == Key.ScanCode) {
        current = (current+1)%NUM;
        }
      gST -> ConOut -> SetCursorPosition(gST -> ConOut,LINEX,LINEY+current);
         			
		}
		if (CHAR_CARRIAGE_RETURN == Key.UnicodeChar) {
      gST -> ConOut -> ClearScreen(gST->ConOut);
			gST -> ConOut -> SetCursorPosition(gST -> ConOut,3,3);
      val = pcideviceval(Bus[current],Dev[current],Fun[current]);
		  }
    if(SCAN_ESC == Key.ScanCode){
      return EFI_SUCCESS;
    }
	}
  return EFI_SUCCESS;             
}
    
    
  

