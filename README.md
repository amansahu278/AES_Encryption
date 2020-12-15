# AES_Encryption
AES Encryption and Decryption on any file.
---
## Description:
   ECB based AES implementation for 128 bit, 192 bit and 256 bit key length.  
   Works almost perfectly for all files (as far as i have used/tested).

## Installation:
   * Clone the repository
   * cd AES_Encryption && make project
   
## Usage:
   * ./AES [-de?V] [-m INT] [--decrypt] [--encrypt] [--mode=INT] [--help] [--usage] [--version] Filepath
   * Filepath is the relative path of the file to be encrypted or decrypted
   * The defaults are encryption and 128bit mode
   * Use ./AES --help or ./AES --h for help  
   * If file name is abc.xyz, the encrypted filename will be abc.xyz_enc
   * If the encrypted file name is abc.xyz, the decrypted filename will be abc.xyz  
   * ### IMPORTANT: if the original file and encrypted file are in the same directory, decryption of the encrypted file WILL OVERWRITE the original file. It is suggested to relocate the orginal file, or rename the original file before decrypting.

### Future Works:
  1. [ ] Threading?
  2. [X] Various key sizes
  3. [X] Change to file stream
  4. [X] Remove padding after decryption
  5. [X] Arg parse
  6. [X] Add progress indicator


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
