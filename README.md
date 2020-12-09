# AES_Encryption
AES Encryption and Decryption on any file.
---
## Description:
   ECB based AES implementation for 128 bit, 192 bit and 256 bit key length. 
   Works almost perfectly for all files (the catch being that the decrypted file >10bytes larger than the original file) 
   _This is because of padding bytes that are added while encryption. This is to be fixed soon._

## Installation:
   * Clone the repository
   * cd AES_Encryption && make project
   
## Usage:
   * Use ./AES --help or ./AES -h for help

### Future Works:
  1. [X] Various key sizes
  2. [X] Change to file stream
  3. [ ] Remove padding after decryption
  4. [X] Arg parse
  5. [ ] Add progress indicator


## Learn to do it yourself:
### Resources: 

   * What is AES
      * https://www.lri.fr/~fmartignon/documenti/systemesecurite/5-AES.pdf
      * https://www.comparitech.com/blog/information-security/what-is-aes-encryption/#How_does_AES_work
      * https://www.youtube.com/watch?v=gP4PqVGudtg
   
   * MixColumns
      * http://www.angelfire.com/biz7/atleast/mix_columns.pdf
      * https://en.wikipedia.org/wiki/Rijndael_MixColumns
      * http://www.infosecwriters.com/text_resources/pdf/AESbyExample.pdf
   
   * Padding of plain text
      * https://www.cryptosys.net/pki/manpki/pki_paddingschemes.html
      * https://www.di-mgt.com.au/cryptopad.html#exampleaes
   
   * Structuring a C project
      * https://dev.to/narasimha1997/understanding-c-c-build-system-by-building-a-simple-project-part-1-4fff
      * https://seenaburns.com/building-c-programs/
   
   * Argument parsing
      * https://www.gnu.org/software/libc/manual/html_node/Argp.html
      * https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html
   
   * Miscellaneous
      * https://www.dummies.com/programming/c/how-to-share-variables-between-modules-when-programming-in-c/
      * https://stackoverflow.com/questions/1433204/how-do-i-use-extern-to-share-variables-between-source-files

### Author: Aman Sahu
