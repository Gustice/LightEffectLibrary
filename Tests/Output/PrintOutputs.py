import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Plot State-Machine Output
df1 = pd.read_csv('Tests/Output/EffectStateMachine.csv')
df2 = pd.read_csv('Tests/Output/EffectSequenceStateMachine.csv')
plt.plot(df1['cycle'], df1['value'], label='longRun')
plt.plot(df2['cycle'], df2['value'], label='alternativ')
plt.xlabel('iteration')
plt.ylabel('intensity')
plt.title('LongRun')
plt.legend()
plt.show()

# Plot Processor Output
df3 = pd.read_csv('Tests/Output/EffectProcessorRun.csv')
plt.plot(df3['cycle'], df3['R'], color='red')
plt.plot(df3['cycle'], df3['G'], color='green')
plt.plot(df3['cycle'], df3['B'], color='blue')
plt.plot(df3['cycle'], df3['RGB'], color='grey')
plt.show()
