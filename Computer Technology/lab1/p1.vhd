---------------------------------
--ELECTR�NICA DIGITAL
--PR�CTICA 1
---------------------------------

--------------------------------
--Librer�as
-------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;

--------------------------------
--Entidad
-------------------------------
ENTITY P1 IS
        PORT (
				a: IN      	STD_LOGIC;
				b: IN      	STD_LOGIC;
				c: IN		STD_LOGIC;
				d: IN		STD_LOGIC;				
				z1:OUT  	STD_LOGIC
        );
END P1;

--------------------------------
--Arquitectura 
-------------------------------
ARCHITECTURE BEH OF P1 IS
--Se�ales 
SIGNAL s1, s2: STD_LOGIC;
SIGNAL s3: STD_LOGIC_VECTOR(1 DOWNTO 0);

BEGIN

	s1<=a xnor b;
	s2<=(b and a);
	s3<=c&d;
	
Proc1: PROCESS (s1, s2, s3)
	BEGIN
		CASE s3 IS
			WHEN "00" => 	z1 <= s2;
			WHEN "01" => 	z1 <= '1';
			WHEN "10" => 	z1 <= '0';
			WHEN OTHERS => 	z1 <= s1;
		END CASE;
	END PROCESS;

END BEH;
