library IEEE;

use IEEE.STD_LOGIC_1164.ALL;

use IEEE.STD_LOGIC_ARITH.ALL;

use IEEE.STD_LOGIC_UNSIGNED.ALL;

 
 

entity key is

port(                  data: in std_logic;--serial data in
                       pclk: in std_logic;
                       CLK: in std_logic;-- for displaying the results
                       segment7 : out std_logic_vector(7 downto 0);
							  AN3,AN2,AN1,AN0:inout std_logic;--signals for activating the 4 LED 7 segment display
                       start1,parity1,stop1 : out std_logic--for the various signals that come preceeding and suceeding the code
                       
								);

end key;

architecture behave of key is

type state is (s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11);
signal present_state,next_state:state;
signal input_code,temp_code:std_logic_vector(7 downto 0):=X"00";
signal start,parity,stop,break:std_logic;
signal CTR :std_logic_vector(12 downto 0);
signal count: integer :=0;
signal input_buffer :std_logic_vector(799 downto 0):=(others =>'1');
signal sft,caps,invalid :std_logic:='0';


begin

process(pclk)
variable tmp_segment7:std_logic_vector(7 downto 0);
begin

if pclk'event and pclk = '1' then
present_state <= next_state;
end if;	


if pclk'event and pclk = '0' then
if present_state = s1 then
start <= data;
next_state <= s2;
--enter data
elsif present_state=s2 then
temp_code(0)<=data;
next_state<=s3;
elsif present_state=s3 then
temp_code(1)<=data;
next_state<=s4;
elsif present_state=s4 then
temp_code(2)<=data;
next_state<=s5;
elsif present_state=s5 then
temp_code(3)<=data;
next_state<=s6;
elsif present_state=s6 then
temp_code(4)<=data;
next_state<=s7;
elsif present_state=s7 then
temp_code(5)<=data;
next_state<=s8;
elsif present_state=s8 then
temp_code(6)<=data;
next_state<=s9;
elsif present_state=s9 then
temp_code(7)<=data;
next_state<=s10;
--parity and stop bit
elsif present_state = s10 then
parity <= data;
next_state <= s11;
--other signals
start1<=start;
parity1<=parity;
stop1<=stop;
--if invalid code and its stop code comes

elsif present_state = s11 then
	--if its a break code
	if (  temp_code=X"F0" ) then
		break<='1';
	else
		break<='0';
	end if;
	--for shift
	if  sft='0' and ( temp_code = X"12" or  temp_code = X"59") then 
		sft <='1';
	end if;
	if sft='1' and  ( temp_code = X"12" or  temp_code = X"59") then
		sft<='0';
	end if;
       
	--if  its not a break code
	if break='0' then
		--for caps
		if temp_code=X"58" then
			if(caps='1') then
				caps<='0';
			else
				caps<='1';
			end if;
		--for backspace
		elsif temp_code=X"66" then
			input_buffer<=  input_buffer(791 downto 0)&"11111111"  ;
		--for display of code
		elsif (sft='1' xnor caps='1') then
			case  temp_code is
				when "01000101"=> tmp_segment7 :="10000001";  -- '0'
				when "00010110"=> tmp_segment7 :="11001111";  -- '1'
				when "00011110"=> tmp_segment7 :="10010010";  -- '2'
				when "00100110"=> tmp_segment7 :="10000110";  -- '3'
				when "00100101"=> tmp_segment7 :="11001100";  -- '4'
				when "00101110"=> tmp_segment7 :="10100100";  -- '5'
				when "00110110"=> tmp_segment7 :="10100000";  -- '6'
				when "00111101"=> tmp_segment7 :="10001111";  -- '7'
				when "00111110"=> tmp_segment7 :="10000000";  -- '8'
				when "01000110"=> tmp_segment7 :="10000100";  -- '9' 
				when "01001110"=> tmp_segment7 :="11111110";  -- '-'
				when "00100100"=> tmp_segment7 :="10010000";  -- 'e'
				when "01010101"=> tmp_segment7 :="11110110";  -- '='
				when "00111100"=> tmp_segment7 :="11100011";  -- 'u'
				when "01000100"=> tmp_segment7 :="11100010";  -- 'o'
				when "01000011"=> tmp_segment7 :="11101111";  -- 'i'
				when "00011100"=> tmp_segment7 :="10000010";  -- 'a'
				when "00011011"=> tmp_segment7 :="10100100";  -- 'S'
				when "00100011"=> tmp_segment7 :="11000010";  -- 'd'
				when "00110011"=> tmp_segment7 :="11101000";  -- 'h'
				when "01001011"=> tmp_segment7 :="11001111";  -- 'l'
				when "00100001"=> tmp_segment7 :="11110010";  -- 'c'
				when "00110010"=> tmp_segment7 :="11100000";  -- 'b'
				when "00010101"=> tmp_segment7 :="10001100";  -- 'q'
				when "00110101"=> tmp_segment7 :="11000100";  -- 'y'
				when "00110100"=> tmp_segment7 :="10000100";  -- 'g'
				when "00110001"=> tmp_segment7 :="11101010";  -- 'n'
				when others=> tmp_segment7 :="11111111";
			end case;
			if tmp_segment7/="11111111" then
				input_buffer<= tmp_segment7 & input_buffer(799 downto 8);
			end if;
		elsif (caps='1' xor sft='1') then
			case  temp_code is
				when "01000101"=> tmp_segment7 :="10000001";  -- '0'
				when "00010110"=> tmp_segment7 :="11001111";  -- '1'
				when "00011110"=> tmp_segment7 :="10010010";  -- '2'
				when "00100110"=> tmp_segment7 :="10000110";  -- '3'
				when "00100101"=> tmp_segment7 :="11001100";  -- '4'
				when "00101110"=> tmp_segment7 :="10100100";  -- '5'
				when "00110110"=> tmp_segment7 :="10100000";  -- '6'
				when "00111101"=> tmp_segment7 :="10001111";  -- '7'
				when "00111110"=> tmp_segment7 :="10000000";  -- '8'
				when "01000110"=> tmp_segment7 :="10000100";  -- '9' 
				when "01001110"=> tmp_segment7 :="11111110";  -- '-'
				when "00100100"=> tmp_segment7 :="10110000";  -- 'E'
				when "01010101"=> tmp_segment7 :="11110110";  -- '='
				when "00111100"=> tmp_segment7 :="11000001";  -- 'U'
				when "01000100"=> tmp_segment7 :="10000001";  -- 'I'
				when "01000011"=> tmp_segment7 :="11001111";  -- 'O'
				when "01001101"=> tmp_segment7 :="10011000";  -- 'P'
				when "00011100"=> tmp_segment7 :="10001000";  -- 'A'
				when "00011011"=> tmp_segment7 :="10100100";  -- 'S'
				when "00100011"=> tmp_segment7 :="10000001";  -- 'D'
				when "00101011"=> tmp_segment7 :="10111000";  -- 'F'
				when "00110011"=> tmp_segment7 :="11001000";  -- 'H'
				when "00111011"=> tmp_segment7 :="11000011";  -- 'J'
				when "01001011"=> tmp_segment7 :="11110001";  -- 'L'
				when "00100001"=> tmp_segment7 :="10110001";  -- 'C'
				when "00110010"=> tmp_segment7 :="10000000";  -- 'B'
				when others=> tmp_segment7 :="11111111";
			end case;
			if tmp_segment7/="11111111" then
				input_buffer<= tmp_segment7 & input_buffer(799 downto 8);
			end if;
		end if ;
	end if;--for break/=1
	stop <= data;
	next_state <= s1;
end if;--for presentstate=s11;
end if;-- pclk'event
end process;



--for displaying the results
process (CLK)
  begin
    if CLK'event and CLK = '1' then
      if (CTR="0000000000000") then
        if (AN0='0') then 
          AN0 <= '1';	 
         segment7 <= input_buffer(783 downto 776);            
          AN1 <= '0';
        elsif (AN1='0') then 
          AN1 <= '1';	 	 
       segment7 <= input_buffer(791 downto 784);           
          AN2 <= '0';
        elsif (AN2='0') then 
          AN2 <= '1';	 
         segment7 <= input_buffer(799 downto 792);            
          AN3 <= '0';
        elsif (AN3='0') then 
          AN3 <= '1';
         segment7 <=input_buffer(775 downto 768);             
          AN0 <= '0';
        end if;
      end if;
      CTR<=CTR+"0000000000001";
      if (CTR > "1000000000000") then   -- counter reaches 2^13
        CTR<="0000000000000";
      end if;
    end if; 
	 
  end process;


end behave;



