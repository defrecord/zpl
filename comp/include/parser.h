#ifndef YYERRCODE
#define YYERRCODE 256
#endif

#define TARRAY 257
#define TBEGIN 258
#define TBLOCK 259
#define TBOOLEAN 260
#define TBREAD 261
#define TBWRITE 262
#define TCHAR 263
#define TCOMPLEX 264
#define TCONFIG 265
#define TCONST 266
#define TCONTINUE 267
#define TCYCLIC 268
#define TDCOMPLEX 269
#define TDIRECTION 270
#define TDO 271
#define TDISTRIBUTION 272
#define TDOUBLE 273
#define TDOWNTO 274
#define TELSE 275
#define TELSIF 276
#define TEND 277
#define TEXIT 278
#define TEXTERN 279
#define TFILE 280
#define TFOR 281
#define TGENERIC 282
#define TGENERICENSEMBLE 283
#define TGRID 284
#define THALT 285
#define TIF 286
#define TINT 287
#define TIRREGULAR 288
#define TLONG 289
#define TMSCAN 290
#define TMULTIBLOCK 291
#define TNONDIST 292
#define TOPAQUE 293
#define TPROCEDURE 294
#define TPROGRAM 295
#define TPROTOTYPE 296
#define TQCOMPLEX 297
#define TQUAD 298
#define TREAD 299
#define TRECORD 300
#define TREFLECT 301
#define TREGION 302
#define TREPEAT 303
#define TRETURN 304
#define TSBYTE 305
#define TSCALEDBY 306
#define TSHORT 307
#define TSTRING 308
#define TTHEN 309
#define TTO 310
#define TTYPE 311
#define TFREE 312
#define TUNION 313
#define TUNSBYTE 314
#define TUNSINT 315
#define TUNSLONG 316
#define TUNSSHORT 317
#define TUNTIL 318
#define TVAR 319
#define TWHILE 320
#define TWRAP 321
#define TWRITE 322
#define TWRITELN 323
#define TINOUT 324
#define TOUT 325
#define TSTATIC 326
#define TLP 327
#define TRP 328
#define TLCBR 329
#define TRCBR 330
#define TLSBR 331
#define TRSBR 332
#define TCOLON 333
#define TCOMMA 334
#define TDOTDOT 335
#define TQUEST 336
#define TQUOTE 337
#define TSEMI 338
#define TNOT 339
#define TRGRID 340
#define TIDENT 341
#define TTYPEIDENT 342
#define TUSRREDUCE 343
#define TUSRSCAN 344
#define TANDREDUCE 345
#define TMAXREDUCE 346
#define TMINREDUCE 347
#define TORREDUCE 348
#define TPLUSREDUCE 349
#define TTIMESREDUCE 350
#define TANDSCAN 351
#define TMAXSCAN 352
#define TMINSCAN 353
#define TORSCAN 354
#define TPLUSSCAN 355
#define TTIMESSCAN 356
#define TBORREDUCE 357
#define TBANDREDUCE 358
#define TBXORREDUCE 359
#define TBORSCAN 360
#define TBANDSCAN 361
#define TBXORSCAN 362
#define TSTR 363
#define TINTEGER 364
#define TFLOAT 365
#define TCHARACTER 366
#define TNOSEMICHECKS 367
#define TNOENDCHECKS 368
#define TPRESERVE 369
#define TDESTROY 370
#define TASSIGN 371
#define TASSPLUS 372
#define TASSMINUS 373
#define TASSMULT 374
#define TASSDIVIDE 375
#define TASSMODULUS 376
#define TASSAND 377
#define TASSOR 378
#define TREGIONAT 379
#define TBY 380
#define TWITH 381
#define TWITHOUT 382
#define TSWITH 383
#define TSWITHOUT 384
#define TOF 385
#define TIN 386
#define TOR 387
#define TAND 388
#define TLESS 389
#define TGREATER 390
#define TLESSEQUAL 391
#define TGREATEREQUAL 392
#define TEQUAL 393
#define TNOTEQUAL 394
#define TPLUS 395
#define TMINUS 396
#define TSTAR 397
#define TDIVIDE 398
#define TMODULUS 399
#define TFLOOD 400
#define TREDUCE 401
#define TUNARY 402
#define TEXP 403
#define TPERMUTE 404
#define TAT 405
#define TWRAPAT 406
#define TPRIME 407
#define TDOT 408
typedef union	{
  int i;
  double r;
  char ch;
  char* str;
  int* pint;
  exprtype et;
  binop_t ot;
  permutetype_t pt;
  struct symboltable_struct *pst;
  struct dimension_struct *pdl;
  struct symlist_struct *psl;
  struct expr_struct *pe;
  struct datatype_struct *pdt;
  struct statement_struct *ps;
  struct initial_struct *pit;
  struct module_struct *pm;
  dimtypeclass dtc;
  regionclass rgc;
} YYSTYPE;
extern YYSTYPE yylval;
