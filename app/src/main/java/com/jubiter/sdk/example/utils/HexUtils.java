package com.jubiter.sdk.example.utils;


import java.math.BigInteger;

/**
 * Hex-utils.
 *
 * @author romus
 */
public class HexUtils {

    private static final byte[] ENCODE_BYTE_TABLE = {
            (byte) '0', (byte) '1', (byte) '2', (byte) '3', (byte) '4', (byte) '5', (byte) '6', (byte) '7',
            (byte) '8', (byte) '9', (byte) 'a', (byte) 'b', (byte) 'c', (byte) 'd', (byte) 'e', (byte) 'f'
    };

    /**
     * Convert byte array to unsigned array.
     *
     * @param data byte array
     * @return unsigned array
     */
    public static int[] convertToUnit(final byte[] data) {
        int[] converted = new int[data.length];
        for (int i = 0; i < data.length; i++) {
            converted[i] = data[i] & 0xFF;
        }

        return converted;
    }

    /**
     * Convert LE to 64-bit value (unsigned long).
     *
     * @param data data
     * @return 64-bit value (unsigned long)
     */
    public static BigInteger convertFromLittleEndianTo64(final int[] data) {
        BigInteger uLong = new BigInteger("0");
        for (int i = 0; i < 8; i++) {
            uLong = uLong.add(new BigInteger(Integer.toString(data[i])).shiftLeft(8 * i));
        }

        return uLong;
    }

    /**
     * Convert 64-bit (unsigned long) value to LE.
     *
     * @param uLong 64-bit value (unsigned long)
     * @return LE
     */
    public static int[] convertFrom64ToLittleEndian(final BigInteger uLong) {
        int[] data = new int[8];
        BigInteger mod256 = new BigInteger("256");
        for (int i = 0; i < 8; i++) {
            data[i] = uLong.shiftRight((8 * i)).mod(mod256).intValue();
        }

        return data;
    }

    /**
     * Bitwise rotate left.
     *
     * @param value  unsigned long value
     * @param rotate rotate left
     * @return result
     */
    public static BigInteger leftRotate64(final BigInteger value, final int rotate) {
        BigInteger lp = value.shiftRight(64 - (rotate % 64));
        BigInteger rp = value.shiftLeft(rotate % 64);

        return lp.add(rp).mod(new BigInteger("18446744073709551616"));
    }

    public static String byteToHex(byte num) {
        char[] hexDigits = new char[2];
        hexDigits[0] = Character.forDigit((num >> 4) & 0xF, 16);
        hexDigits[1] = Character.forDigit((num & 0xF), 16);
        return new String(hexDigits);
    }

    /**
     * Convert bytes to string.
     *
     * @param byteArray bytes array
     * @return string
     */
    public static String convertBytesToString(final byte[] byteArray) {
        StringBuffer hexStringBuffer = new StringBuffer();
        for (int i = 0; i < byteArray.length; i++) {
            hexStringBuffer.append(byteToHex(byteArray[i]));
        }
        return hexStringBuffer.toString();
    }


    public static byte hexToByte(String hexString) {
        int firstDigit = toDigit(hexString.charAt(0));
        int secondDigit = toDigit(hexString.charAt(1));
        return (byte) ((firstDigit << 4) + secondDigit);
    }

    private static int toDigit(char hexChar) {
        int digit = Character.digit(hexChar, 16);
        if(digit == -1) {
            throw new IllegalArgumentException(
                    "Invalid Hexadecimal Character: "+ hexChar);
        }
        return digit;
    }

    /**
     * Returns a byte array from a string of hexadecimal digits.
     *
     * @param hexString the string to convert to a byte array.
     * @return
     */
    public static byte[] fromString(String hexString) {
        if (hexString.length() % 2 == 1) {
            throw new IllegalArgumentException(
                    "Invalid hexadecimal String supplied.");
        }

        byte[] bytes = new byte[hexString.length() / 2];
        for (int i = 0; i < hexString.length(); i += 2) {
            bytes[i / 2] = hexToByte(hexString.substring(i, i + 2));
        }
        return bytes;
    }

}
