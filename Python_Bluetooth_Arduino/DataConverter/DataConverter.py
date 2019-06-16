def DataConverter(dataArr, expansion):
        res = [0]*len(dataArr)*expansion
        for i in range(len(dataArr)):
            sign = 1
            current_val = dataArr[i]
            if current_val < 0:
                sign = -1
                current_val = -current_val
            for j in range(expansion):
                res[i*expansion+expansion-j-1] = sign*(current_val%100)
                current_val = (current_val - (current_val%100))/100
        return res    

print(DataConverter([5000, 1000, 2000, 4899, -1200, 3200], 3))
