/*
Author: Meghana Gadiraju
Class : CS221
Professor Vahab
Date: November 12
Project 2
*/


#include "matcher.h"
//can't include stdbool.h, so using 1 and 0 

/*
	points to next character
*/
char next(char *str) 
{
	return *(str + sizeof(char));
}

/*
	points to previous character
*/

char last(char *str) 
{
	return *(str - sizeof(char));
}

/*
	Checks to see if the character is included in the characters that we are looking for. 
*/

int is_inc(char pattern) 
{
	if (pattern == '\\'|| pattern == '\?'|| pattern == '.'|| pattern == '+')
	{
		return 1;
	}

	return 0;
}

/*
	looks for question mark
*/

int quest(char *pattern) 
{
	if(next(pattern)  == '\?')
	{
		return 1;
	}
	return 0;
}

/*
	looks for backslash (escape) 
*/
int esc(char *pattern) 
{
	if(last(pattern) == '\\')
	{
		return 1;
	}
	return 0;
}

/*
	looks for plus sign
*/

int plus(char *pattern) 
{
	if(next(pattern) == '+')
	{
		return 1; 
	}
	return 0;
}

/**
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */
int matches_leading(char *partial_line, char *pattern) 
{
	//Checks if partial is same as pattern
	//or checks if pattern equals .escape
	//or checks if partial equals pattern and .escape
	//or checks escape and ?
	if((*partial_line == *pattern && !is_inc(*pattern))||(*pattern == '.' && !esc(pattern)) ||(*partial_line == *pattern && is_inc(*pattern) && esc(pattern))|| (quest(pattern)))
	{
		return 1;
	}
	//Check escape character
	if(*pattern == '\\')
	{
		return matches_leading(partial_line, pattern + sizeof(char));
	}
	return 0;
}

/**
 * Implementation of your matcher function, which
 * will be called by the main program.
 *
 * You may assume that both line and pattern point
 * to reasonably short, null-terminated strings.
 */
int rgrep_matches(char *line, char *pattern) 
{
	
	static int p_len = 0;
	int s = sizeof(char);//makes runtime quicker 

	//Base case for recursive function
	if(*pattern == '\0') 
	{
		//Reset pattern for next line	
		if(*line == '\n') 
		{
			pattern -= p_len * s;
		}
		p_len = 0;
		return 1;
	}
	

	//Reset pattern for new line	
	if(*line == '\n') 
	{
		pattern -= p_len * s;
		p_len -= p_len;
		return 0;
	}

	//Check for escape
	if((*pattern == '\\') && is_inc(next(pattern))) 
	{
		pattern += s;
		p_len++;
	}

	//Check for match
	if(matches_leading(line, pattern)) 
	{
		//check for +
		if(plus(pattern)) 
		{
			int chars_before = 1;//Stops iteration before going to next line
			while(*(line + chars_before * s) == *pattern && !(*pattern == '.' && !esc(pattern)))
			{
				line += s;
			}
			
			pattern += s;
			p_len++;
		}

		//checks for ?
		if(quest(pattern)) 
		{
			//if line equals pattern and if next equals pattern +2
			if(*line == *pattern && next(line) == *(pattern + 2 * s))
			{
				//increments pattern and length
				pattern += s;
				p_len++;
			}
			//checks if line equals pattern and next does not equal pattern +2 and line does not equal pattern +2
			else if(*line == *pattern && next(line) != *(pattern + 2 * s) && *line != *(pattern + 2 * s))
			{
				return 0;
			}
			//checks if pattern = . operator and does not equal escape or line does not equal pattern
			else if(!(*pattern == '.' && !esc(pattern)) || *line != *pattern)
			{
			 	line -= s;
			}
			pattern += s;
			p_len++;
		}
		pattern += s;
		p_len++;

	}

	//Reset pattern for partial matches
	else if(p_len != 0 && *(pattern - p_len * s) != '\0') 
	{
		pattern -= p_len * s;
		p_len -= p_len;
	}

	line += s;

	

	//calling function recursively
	return rgrep_matches(line, pattern);
}
