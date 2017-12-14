# DSV-Analyser

## Description:

Analyse CSV, TSV files for representation to DB format, analysing returns:
1. Columns amount (with fileds title on demand)
2. Each column's maximum field length in bytes
3. Each column's possible data type:
* TEXT -  contains any data 
* NUMBER  -  contains only digits
* DOUBLE  -  contains only digits with decimal separator
* all these types enumerated in **DSV_TYPES**;

Detection of possible datatypes and lengths of the fields implemented by reading/detection byte-by-byte just in time.

## Usage:

`DSV_Analyser obj(filepath, fields_delimiter, decimal_delimiter);`

Instance should be initialized with `filepath`(fullpath to the file), `fields_delimiter`(1 char, for CSV - comma, for TSV - '\t' and etc), `decimal_delimiter`(dot or comma depends of your file);


`obj.Analyse(hasTitles);`


Method Analyse() will open file and start analysing.  `hasTitles `is boolean argument which is true by default,setting this argument to true will append Titles into Columns vector otherwise Titles will be empty in Columns after Analyse completion.  

After analyse we can use obj.Columns for access to analysed information.
`obj.Columns` - vector of DSV_FieldInfo structures. `DSV_FieldInfo` { Title, Length, Type }
`obj.Columns[index].Type` - will return all types that were found in the Column with index while analysing.
> 0x01 - TEXT

> 0x02 - NUMBER

> 0x04 - DOUBLE

So if we need to check does selected Column contains any DOUBLE values or not, we should use bitmask:

`obj.Coulmns[index].Type & DSV_TYPES::DOUBLE_TYPE`

## Additional Information:
