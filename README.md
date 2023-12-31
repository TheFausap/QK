# QK
Quantum computing simulation in K

This project is based on two programming languages I love the most: C and K.  
About K, there are a lot of "variants" or "dialects" around, for this project I needed a K interpreter able to manage complex numbers.
For this reason, I chose https://github.com/ktye/i

The C part is a sort of assembler, using a stack based approach, generating an output for the Q "coprocessor" (written in K), that perform the quantum simulation.

These are the "opcodes" defined:


    // vectors
    #define K0      0x1
    #define K1      0x2
    #define B0      0x3
    #define B1      0x4
    #define P0      0x5
    #define P1      0x6
    // matrices
    #define I       0x10
    #define X       0x11
    #define Z       0x12
    #define H       0x13
    #define Y       0x14
    #define S       0x15
    #define SX      0x16
    #define T       0x17
    #define uCX     0x18
    #define dCX     0x19
    #define M       0x20
    #define ECR     0x21
    #define SWAP    0x22
    #define PG      0x23
    #define CCX     0x24
    // use parameter
    #define Rx      0x30
    #define Ry      0x31
    #define Rz      0x32
    #define Rn      0x33
    #define CP      0x34
    #define U       0x35
    #define P       0x36
    #define QFT     0x37
    // functions or operators
    #define TS      0x40
    #define SP      0x41
    #define NZP     0x42
    #define NZC     0x43
    #define SZ      0x44
    #define BASIS   0x45
    #define KET     0x46
    #define BRA     0x47
    #define PPK     0x48
    #define PPB     0x49
    #define QPP     0x50
    #define PST     0x51
    #define MMU     0x52
    #define APPLY   0x53
    #define MKR     0x54
    #define INV     0x55
    #define MPOWI   0x56
    #define ID      0x57
    #define P2      0x58
    #define PN      0x59
    #define MKRN    0x60
    #define OM      0x61
    #define CNTRL   0x62
    #define PROB    0x63
    #define PPROB   0x64
    #define SHF     0x65
    #define CLK     0x66

The program sytax looks like a KDF9 autocode program.

    K0;K0;DUP;*V;*V;
    I;H;*K;
    I;REV;*K;
    APPLY;I;uCX;*K;APPLY;
    uCX;I;*K;APPLY;=V2;
    SET8;QFT;=V1;
    APPLY;
    [2/4/2];U;I;I;*K;*K;
    V2;REV;APPLY;
    [2/4/9/6];
    MAT2;
    H;{};
    DB;

output:

    G0:ts[K0;K0]
    G1:ts[G0;K0]
    G2:mkr[H;I]
    G3:mkr[G2;I]
    G4:apply[G3;G1]
    G5:mkr[uCX;I]
    G6:apply[G5;G4]
    G7:mkr[I;uCX]
    G8:apply[G7;G6]
    V2:G8
    G9:QFT 8
    V1:G9
    G10:apply[G9;G8]
    G11:U[2;4;2]
    G12:mkr[I;I]
    G13:mkr[G12;G11]
    G14:apply[G13;V2]
    G15:(6.000000 9.000000; 4.000000 2.000000)
    G16:mmu[G15;H]+mmu[H;G15]
    G17:zround ((cos vDB*pi)*mkr[I;I])-(1a90)*(sin vDB*pi)*(mkr[X;X]+mkr[Y;Y])

and it is, of course, more verbose than the original "autocode" source.  
This output is managed by K using the q.k library. 

## CAVEATS

Some operators or functions, defined in the q.k library, are not present in the assembler yet.

## AUTOCODE SYNTAX

Each entry is pushed on to stack, and the result of an function is automatically pushed on to stack.  
These are stack operations, not related to quantum processing:

    DUP
    REV
    SET
    []
    MAT
    QMAT
    =
    +
    -
    *
    /
    CAB
    PERM
    
Some of them are directly deriving from KDF9 autocode.  

    DUP        : duplicate the last element on to stack
    REV        : reverse the two last elements on to stack
    SET<N>     : put the number N on to stack
    [N/N/...]  : put the numbers on to stack
    MAT[2|3]   : create a numeric matrix 2x2 or 3x3, popping from the stack the elements
    QMAT[2|3]  : create a quantum matrix 2x2 or 3x3, popping quantum objects from the stack
    =<V>       : creates a new variable V with a value equals to the last element on the stack
    +,-,*,/    : unsigned integer arithmentic operations between stack elements
    CAB        : performs a permutation ABC -> CAB
    PERM       : performs a permutation ABC -> BCA

All the other operations are mapped to specific operators or functions defined in q.k  
For mathematical details, see https://en.wikipedia.org/wiki/List_of_quantum_logic_gates

These are:

    // vectors
    K0     : |0> 
    K1     : |1> 
    B0     : <0| 
    B1     : <1| 
    P0     : |0><0|
    P1     : |1><1| 
    // matrices
    I      : Identity, NOP gate (one qubit) 
    X      : Pauli X, NOT, bit flip gate (one qubit)
    Z      : Z, phase flip gate (one qubit) 
    H      : Hadamard gate (one qubit) 
    Y      : Y gate (one qubit) 
    S      : S, square root of Z gate(one qubit) 
    SX     : SX, square root of NOT gate (one qubit) 
    T      : T, fourth root of Z gate (one qubit) 
    uCX    : up Controlled-X, Controlled-NOT, XOR gate (two qubits) 
    dCX    : down Controlled-X, Controlled-NOT, XOR gate (two qubits)
    M      : Magic gate (two qubits)
    ECR    : ECR gate (two qubits)
    SWAP   : SWAP gate (two qubits) 
    PG     : Peres gate (three qubits)
    CCX    : Controlled-Controlled-X gate (Toffoli) gate (three qubits)

they are directly related to variables defined in q.k and they are stored directly on to stack.
Then we have:

    // use parameters
    Rx      : Rotation X-axis
    Ry      : Rotation Y-axis
    Rz      : Rotation Z-axis
    Rn      
    CP      : Controlled-phase gate
    U       : General rotation gate
    P       : phase shift gate
    QFT     : Quantum Fourier Transform gate
    CNTRL   : Create a controlled gate (i.e. x=total qubits, y=ctrl qubits, z=tgt qubit, uCX = cntrl[2;1;2;X] / numbered from the top)
    ID      : Identity matrix (multi qubits)

these matrices, define in q.k, require one or more parameters taken from the stack.
Then, we have miscellaneous functions:

    // functions or operators
    TS    : tensor product
    SP     
    NZP   : return non zero positions in the quantum state 
    NZC   : return non zero coefficients in the quantum state  
    SZ    : size of quantum state  
    BASIS   
    KET   : defines a Ket  
    BRA   : defines a Bra  
    PPK   : pretty print of a ket  
    PPB   : pretty print of a bra  
    QPP   : pretty print of quantum state  
    PST     
    MMU   : matrix multiplication 
    APPLY : matrix / vector multiplication  
    MKR   : kronecker product 
    INV     
    MPOWI : matrix power 
    P2    : square  
    PN    : n-power  
    MKRN  : n-times repeated kronecker product 
    OM    : power of complex unity (omega)  
    PROB  : display amplitudes
    PPROB : display percentage of probability 
    SHF   : shift matrix (used for QFT)
    CLK   : clock matrix (used for QFT)

All these functions are related to various operations, mostly internals, used by the Q coprocessor itself. These are not directly available in the Autocode, but are called indirectly.  
Two of these functions are quite important: `prob` and `pprob`.  
For example, this sequence, automatically generated by q-k, performa a QFT (quantum fourier transform) of a three-qubits state

    G0000:ts[K0;K0]
    G0001:ts[G0000;K0]
    G0002:mkr[H;I]
    G0003:mkr[G0002;I]
    G0004:apply[G0003;G0001]
    G0005:mkr[uCX;I]
    G0006:apply[G0005;G0004]
    G0007:mkr[I;uCX]
    G0008:apply[G0007;G0006]
    G0009:QFT 8
    G0010:apply[G0009;G0008]

The matrix `G0009` is a 8x8 matrix, representing the QFT operator for 3 qubits:

    0.353553a 0.353553a 0.353553a 0.353553a 0.353553a 0.353553a 0.353553..
    0.353553a 0.353553a45 0.353553a90 0.353553a135 0.353553a180 0.353553..
    0.353553a 0.353553a90 0.353553a180 0.353553a270 0.353553a 0.353553a9..
    0.353553a 0.353553a135 0.353553a270 0.353553a45 0.353553a180 0.35355..
    0.353553a 0.353553a180 0.353553a 0.353553a180 0.353553a 0.353553a180..
    0.353553a 0.353553a225 0.353553a90 0.353553a315 0.353553a180 0.35355..
    0.353553a 0.353553a270 0.353553a180 0.353553a90 0.353553a 0.353553a2..
    0.353553a 0.353553a315 0.353553a270 0.353553a225 0.353553a180 0.3535..

and the final state of the system is (represented in an internal format):

    G0010
    `ket|0.499999a 0.461939a337.5 0.353553a315 0.191341a292.5 0a 0.191341a67...

and in more human readable format, using probabilities:

     pprob G0010
    ("24.999999%";"|000>")
    ("21.338834%";"|001>")
    ("12.499999%";"|010>")
    ("3.661165%";"|011>")
    ("3.661165%";"|101>")
    ("12.499999%";"|110>")
    ("21.338834%";"|111>")


