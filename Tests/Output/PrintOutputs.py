import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

df = pd.read_csv('Tests/Output/EffectStateMachine.csv')

plt.plot(df['cycle'], df['value'], label='longRun')
plt.xlabel('iteration')
plt.ylabel('intensity')
plt.title('LongRun')
plt.legend()
plt.show()



# Time series plot
# df.plot(x=1, y=2)
# plt.show()
# plt.plot(df['input'], df['output'])
# plt.show()
# plt.plot(df['input'], df['output'], label='y = x^2')
# plt.xlabel('Input')
# plt.ylabel('Output')
# plt.title('Function')
# plt.legend()
# plt.show()


# sns.color_palette("Spectral", as_cmap=True)
# sns.relplot(x='input', y='output',   hue='meaning', data=df)
# plt.show()