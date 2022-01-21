----------------------------------------------------------------------------------
-- Company: 
-- Engineer: TalosElectrónico
-- 
-- Create Date:    18:59:29 01/19/2022 
-- Design Name: 
-- Module Name:    RobotVHDL - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity RobotVHDL is
    Port ( S1 : in  STD_LOGIC;
           S2 : in  STD_LOGIC;
           CLK : in  STD_LOGIC;
           PWMa : out  STD_LOGIC;
           PWMb : out  STD_LOGIC;
           Reset : in  STD_LOGIC);
end RobotVHDL;

architecture Behavioral of RobotVHDL is

begin

p1: process (CLK, Reset)
variable Contador:integer range 0 to 1000000:=0; --Variable de conteo para el periodo T para cada servomotor (20ms).
variable ComparacionS1: integer range 0 to 125000;
variable ComparacionS2: integer range 0 to 125000;
begin

if(CLK='1' and CLK'event)then

if (Reset='1')then --Reset de la entidad
	PWMa<='0';
	PWMb<='0';
	Contador:=0;
end if;

Contador:=Contador+1; --Incremento de contador en una unidad.

if (Contador>=1000000)then
Contador:=0;
end if;

if(S1='0' and S2='0')then
	ComparacionS1:=25000; --0.5ms (0°)
	ComparacionS2:=25000; --0.5ms (0°)
elsif(S1='0' and S2='1')then
	ComparacionS1:=125000; --2.5ms (180°)
	ComparacionS2:=25000;  --0.5ms (0°)
elsif(S1='1' and S2='0')then
	ComparacionS1:=25000; --0.5ms (0°)
	ComparacionS2:=125000;  --2.5ms (180°)
elsif(S1='1' and S2='1')then
	ComparacionS1:=125000; --2.5ms  (180°)
	ComparacionS2:=125000;  --2.5ms (180°)
end if;

if(Contador<=ComparacionS1)then --Si contador es menor al valor de comparación la salida en 1
PWMa<='1';
else
PWMa<='0';
end if;

if(Contador<=ComparacionS2)then
PWMb<='1';
else
PWMb<='0';
end if;

end if;
end process;
end Behavioral;
