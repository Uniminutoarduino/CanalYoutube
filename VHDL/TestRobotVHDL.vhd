--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   19:26:38 01/19/2022
-- Design Name:   
-- Module Name:   C:/Users/Jhonatan/Documents/Xilinx Projects/PWMRobot/TestRobot.vhd
-- Project Name:  PWMRobot
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: RobotVHDL
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY TestRobot IS
END TestRobot;
 
ARCHITECTURE behavior OF TestRobot IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT RobotVHDL
    PORT(
         S1 : IN  std_logic;
         S2 : IN  std_logic;
         CLK : IN  std_logic;
         PWMa : OUT  std_logic;
         PWMb : OUT  std_logic;
         Reset : IN  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal S1 : std_logic := '0';
   signal S2 : std_logic := '0';
   signal CLK : std_logic := '0';
   signal Reset : std_logic := '0';

 	--Outputs
   signal PWMa : std_logic;
   signal PWMb : std_logic;

   -- Clock period definitions
   constant CLK_period : time := 20 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: RobotVHDL PORT MAP (
          S1 => S1,
          S2 => S2,
          CLK => CLK,
          PWMa => PWMa,
          PWMb => PWMb,
          Reset => Reset
        );

   -- Clock process definitions
   CLK_process :process
   begin
		CLK <= '0';
		wait for CLK_period/2;
		CLK <= '1';
		wait for CLK_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;	

      wait for CLK_period*10;

      -- insert stimulus here 
		S1<='0';
		S2<='0';
		wait for 100 ms;	
		S1<='0';
		S2<='1';
		wait for 100 ms;	
		S1<='1';
		S2<='0';
		wait for 100 ms;	
		S1<='1';
		S2<='1';

      wait;
   end process;

END;
