
110-2 ALCO Project 2 - 3-bit Histroy Predictor
===

2-bit Counter State Diagram
---
![](https://i.imgur.com/t12c9VC.png)


                    
3-bit Histroy Predictor
---
共有$2^3$個2-bit Counter
![](https://i.imgur.com/HLlAjlR.png)



Input & Output
---
* 低分版: TTTTNNTNTNTNTNTNT
* 高分版(可指定entry數量): 

```gherkin=
	li R2,0			    # v=0 		
    li R3,16		    #  Loop bound for LoopI 
	li R4,0			    #  i=0 
LoopI:
	beq R4,R3,EndLoopI	# Exit LoopI if i==16
	li R5,0			    # j=0 
LoopJ:
	beq R5,R3,EndLoopJ  # Exit LoopJ if j==16
	add R6,R5,R4	    # j+i
	andi R6,R6,3	    # (j+i)%4
	bne R6,R0,Endif	    # Skip if (j+i)%4!=0
	add R2,R2,R5        # v+=j
Endif:
	addi R5,R5,1        # j++
	beq R0,R0,LoopJ	    # Go back to LoopJ
EndLoopJ:
	addi R4,R4,1        # i++
	beq R0,R0,LoopI	    # Go back to LoopI
EndLoopI:

```



```gherkin=
Feature: Shopping Cart
  As a Shopper
  I want to put items in my shopping cart
  Because I want to manage items before I check out

  Scenario: User adds item to cart
    Given I'm a logged-in User
    When I go to the Item page
    And I click "Add item to cart"
    Then the quantity of items in my cart should go up
    And my subtotal should increment
    And the warehouse inventory should decrement
```

> Read more about Gherkin here: https://docs.cucumber.io/gherkin/reference/


