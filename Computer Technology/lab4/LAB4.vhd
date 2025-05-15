library ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;

ENTITY lab4 IS
	PORT(
		Button, Reset, Clk: IN STD_LOGIC;
		RAG, Count: OUT STD_LOGIC_VECTOR(2 DOWNTO 0)
	);
END lab4;

ARCHITECTURE functional OF lab4 IS
type state IS(start, first, second, third);
SIGNAL current_state, next_state: state;
SIGNAL q1, q2, Button_E, E, F: STD_LOGIC;
SIGNAL Count_s: UNSIGNED(2 DOWNTO 0);
	
BEGIN
Count <= STD_LOGIC_VECTOR(Count_s);

-- Edge detector
PROCESS(Clk,Reset)
BEGIN
	IF Reset = '0' THEN
		q1 <= '0';
		q2 <= '0';
	ELSIF Clk'event and Clk = '1' THEN
		q1 <= Button;
		q2 <= q1;
	END IF;
END PROCESS;

Button_E <= (not(q2)) and (q1);

-- Timer
F <= '1' WHEN Count_s = "100" ELSE '0';
PROCESS(Clk,Reset,E,Count_s)
BEGIN
	IF Reset = '0' THEN
		Count_s <= (OTHERS=> '0');
	ELSIF Clk'event and Clk = '1' THEN
		IF E = '1' THEN
			IF Count_s = "100" THEN
				Count_s <= "000";
			ELSE
				Count_s <= Count_s + 1;
			END IF;
		END IF;
	END IF;
END PROCESS;

-- Fsm
PROCESS(Clk, Reset)
BEGIN
	IF Reset = '0' THEN
		current_state <= start;
	ELSIF Clk'EVENT AND Clk='1' THEN
		current_state <= next_state;
	END IF;
END PROCESS;

PROCESS(current_state, Button_E, F)
BEGIN
	CASE current_state IS
		WHEN start =>
			RAG <= "001";
			E <= '0';
			IF Button_E = '1' THEN
				next_state <= current_state;
			ELSE
				E <= '1';
				next_state <= first;
			END IF;
		WHEN first =>
			RAG <= "001";
			E <= '1';
			IF F = '0' THEN
				next_state <= current_state;
			ELSE
				next_state <= second;
			END IF;
		WHEN second =>
			RAG <= "010";
			E <= '1';
			IF F = '0' THEN
				next_state <= current_state;
			ELSE
				next_state <= third;
			END IF;
		WHEN third =>
			RAG <= "100";
			E <= '1';
			IF F = '0' THEN
				next_state <= current_state;
			ELSE
				next_state <= start;
			END IF;
	END CASE;
END PROCESS;
END functional;
