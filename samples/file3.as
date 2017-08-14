
void strToBinWord(char *str,AddressMode mode,int op_type,int passage){
    int bits = 0;
    struct {
        unsigned is_srcop_reg:2;
        unsigned is_destop_reg:2;
    }flag = {FALSE,FALSE};
    symbolTable *symbolId;
    char label[MAX_LINE];
    char arg1[MAX_LINE],arg2[MAX_LINE];

    switch(mode){
        case ADDMODE_IMMEDIATE:
            bits = atoi(&str[1]);
            if(bits < 0)
                bits = ~((-1)*bits) + 1;
            return bits<<2;
        case ADDMODE_DIRECT:
            if(passage == SECOND_PASS && (symbolId = get_symbolId(str)) != NULL)
                return (symbolId->address << 2) | (symbolId->position);
            else
                return 0;
        case ADDMODE_MATRIX:
            strncpy(label,str,strchr(str,'[')-str);
            if(passage == FIRST_PASS)
                ic++;       /* no label adress yet, progressing one step */
            else if(passage == SECOND_PASS){
                bits |=
            }
            cpyMatVals(str,arg1,arg2);
            return (atoi(&arg1[1])<<6) | atoi(&arg2[1])<<2;
        case ADDMODE_REG:
            if(op_type==SRC_OP)
                flag.is_srcop_reg = TRUE;
            if(op_type==DEST_OP)
                flag.is_destop_reg = TRUE;
            bits = atoi(&str[1]);
            bits<<(op_type == SRC_OP?6:2);
        case ADDMODE_NO_OPERAND:
            return 0;
        default:
            return 0;
    }

}
