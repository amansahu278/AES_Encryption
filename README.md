# AES_Encryption
AES Encryption and Decryption on any file.
---
   ECB AES implementation for 128 bit, 192 bit and 256 bit key length. Works almost perfectly for all files.
   // Decrypting an encrypted file leads to increase in size of the decrypted file by a few bits (because of padding bits). This is to be fixed soon.

## How to use:
  

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
