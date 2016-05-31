<?php
/**
 * Сериализатор для TLV-формата
 *
 * @author Dmitriy Khaustov aka Dиmon
 */
class TLVEncoder
{
	/**
	 * Сериализованые данные
	 * @var string
	 */
	private $tlvData;

	/**
	 * 
	 * @var unknown_type
	 */
	private $antiRecursion;

	/**
	 * 
	 * @param mixed $value
	 */
	public function __construct($value)
	{
		$this->tlvData = "";
			
		$this->antiRecursion = array();
		$this->encodeValue($value);
		$this->antiRecursion = null;
	}

	public function getData()
	{
		return $this->tlvData;
	}

	/**
	 * Кодирует значение
	 * @param mixed $value входные данные
	 */
	private function encodeValue($value)
	{
		if (is_string($value))
		{
			$this->tlvData .= pack("C", TLV::STRING);
			$this->tlvData .= $value;
			$this->tlvData .= "\x00";
		}
		else if (is_int($value))
		{
			if ($value > 0xFFFF)
			{
				$this->tlvData .= pack("C", TLV::UINT_32);
				$this->tlvData .= pack("N", $value);
			}
			else if ($value > 0xFF)
			{
				$this->tlvData .= pack("C", TLV::UINT_16);
				$this->tlvData .= pack("n", $value);
			}
			else if ($value > 0)
			{
				$this->tlvData .= pack("C", TLV::UINT_8);
				$this->tlvData .= pack("C", $value);
			}
			else if ($value < -0x8FFF)
			{
				$this->tlvData .= pack("C", TLV::INT_32);
				$this->tlvData .= pack("N", -$value);
			}
			else if ($value < -0x8F)
			{
				$this->tlvData .= pack("C", TLV::INT_16);
				$this->tlvData .= pack("n", -$value);
			}
			else if ($value < 0)
			{
				$this->tlvData .= pack("C", TLV::INT_8);
				$this->tlvData .= pack("C", -$value);
			}
			else
			{
				$this->tlvData .= pack("C", TLV::ZERO);
			}
		}
		else if ($value === true)
		{
			$this->tlvData .= pack("C", TLV::TRUE_VAL);
		}
		else if ($value === false)
		{
			$this->tlvData .= pack("C", TLV::FALSE_VAL);
		}
		else if (is_array($value))
		{
			foreach ($value as $key => $arrayElement)
			{
				if (is_string($key))
				{
					$this->encodeAssocArray($value);
					return;
				}
			}
			
			$this->tlvData .= pack("C", TLV::ARRAY_VAL);
			foreach ($value as $arrayElement)
			{
				$this->encodeValue($arrayElement);
			}
			$this->tlvData .= pack("C", TLV::END);
		}
		else if (is_object($value) && $value != null)
		{
			$this->encodeObject($value);
		}
		else if ($value === null)
		{
			$this->tlvData .= pack("C", TLV::NULL_VAL);
		}
		else if (is_nan($value))
		{
			$this->tlvData .= pack("C", TLV::NAN);
		}
		else
		{
			$this->tlvData .= pack("C", TLV::UNKNOWN);
		}
	}

	/**
	 * Кодирует объект целиком
	 */
	private function encodeAssocArray($array)
	{
		$this->tlvData .= pack("C", TLV::OBJECT_VAL);
		
		foreach ($array as $key => $value)
		{
			// Записываем индекс...
			$this->tlvData .= "{$key}";
			$this->tlvData .= "\x00";

			// ...и значение свойства
			$this->encodeValue($value);
		}
		
		$this->tlvData .= pack("C", TLV::END);
	}
	
	/**
	 * Кодирует объект целиком
	 */
	private function encodeObject($object)
	{
		// Сохраняем объект в список обработанных объектов,
		// чтобы иметь возможность прервать рекурсивное зацикливание
//		$this->antiRecursion[$object] = true;

		$this->tlvData .= pack("C", TLV::OBJECT_VAL);

		// Проверяем, является ли объект экземпляром класса или простым объектом
		$className = get_class($object);
		
		if ($className == "stdClass")
		{
			// Перебираем свойства объекта
			foreach ($object as $key => $value)
			{
				// Пропускаем, если свойства-функции
				if (is_callable(array($object, $key)) || is_callable($value))
				{ 
					continue;
				}

				// Записываем имя свойства...
				$this->tlvData .= $key;
				$this->tlvData .= "\x00";

				// ...и значение свойства
				$this->encodeValue($value);
			}
		}
		// Экземпляр сласса
		else
		{
			$reflection = new ReflectionClass($className);

			// Получаем все доступные свойства (простые поля и геттеры)
			$list = array_merge(
				$reflection->getConstants(),
				$reflection->getProperties(),
				$reflection->getStaticProperties()
			);
			
			// обрабатываем полученный список
			foreach ($list as $key)
			{
				$this->tlvData .= $key;
				$this->tlvData .= "\x00";

				// Пробуем получить значение
				try
				{
					$value = $object->$key;
				}
				catch(Exception $e)
				{
					// Если недоступно, пишем соответствующее значение
					$this->tlvData .= pack("C", TLV::UNAVAILABLE);
					continue;
				}
					
				// Проверяем, обрабатывались ли уже эти данные
				if ($this->antiRecursion[$value])
				{
					// Обрабатывались. Пишем флаг прерывания рекурсии
					$this->tlvData .= pack("C", TLV::RECURSION);
				}
				else
				{
					// Декодируем значение
					$this->encodeValue($value);
				}
			}
		}

		// Пишем флаг окончания данных объекта
		$this->tlvData .= pack("C", TLV::END);
	}
}

/**
 * Десериализатор для TLV-формата
 *
 * @author Dmitriy Khaustov aka Dиmon
 */
class TLVDecoder
{
	private $tlvData = "";
	private $tlvDataPos = 0;
	private $tlvDataLen = 0;
	
	private $strict;

	/**
	 * TLV-Десериализатор
	 *
	 * @param encodedData Массив закодированых данных
	 * @param strict Флаг необходимости сгенерировать ошибку, если встретится несоответствие формату.
	 */
	public function __construct($encodedData, $strict = true)
	{
		$this->tlvData = $encodedData;
		$this->strict = $strict;

		$this->tlvDataPos = 0;
		$this->tlvDataLen = strlen($this->tlvData);
		
		$this->value = $this->nextToken();

		if (is_a($this->value, "TLVEndToken"))
		{
			$this->generateParseError("Unexpected 'TLVEndToken' when root token reading", 0);
			$this->value = null;
		}
	}

	private $error;
	/**
	 * Ошибка парсинга
	 */
	public function getError()
	{
		return $this->error;
	}

	/**
	 * Генерация ошибки парсинга
	 */
	private function generateParseError($message, $offset)
	{
		$error = new stdClass();
		$error->message = $message;
		$error->offset = $offset;
			
		if ($this->error == null)
		{
			$this->error = $error;
		}

		if ($this->strict) throw(new Exception("Error '{$message}' in offset '{$offset}'"));
	}

	private $value;
	/**
	 * @return Декодированые данные
	 */
	public function getValue()
	{
		return $this->value;
	}

	/**
	 * Извлекает очередное значение. Читает байт типа, и на его основе считывает и декодирует значение, если требуется.
	 */
	private function nextToken()
	{
		if ($this->tlvDataPos == $this->tlvDataLen)
		{
			$this->generateParseError("Unexpected end of data when token type reading", $this->tlvDataPos);
			return null;
		}

		$a = unpack("@{$this->tlvDataPos}s/C1d", $this->tlvData);
		$tvlTokenType = $a["d"];
		$this->tlvDataPos++;
		
		switch ($tvlTokenType)
		{
			case TLV::NULL_VAL:
				return null;

			case TLV::FALSE_VAL:
				return false;
					
			case TLV::TRUE_VAL:
				return true;
					
			case TLV::NAN:
				return NaN;
					
			case TLV::ZERO:
				return 0;
					
			case TLV::INT_8:
				if ($this->tlvDataPos + 1 > $this->tlvDataLen)
				{
					$this->generateParseError("Unexpected end of data when byte data reading", $this->tlvDataPos);
					return null;
				}
				$a = unpack("@{$this->tlvDataPos}s/c1d", $this->tlvData);
				$this->tlvDataPos++;
				return $a["d"];

			case TLV::UINT_8:
				if ($this->tlvDataPos + 1 > $this->tlvDataLen)
				{
					$this->generateParseError("Unexpected end of data when unsinged byte data reading", $this->tlvDataPos);
					return null;
				}
				
				$a = unpack("@{$this->tlvDataPos}s/C1d", $this->tlvData);
				$this->tlvDataPos++;
				return $a["d"];
									
			case TLV::INT_16:
				if ($this->tlvDataPos + 2 > $this->tlvDataLen)
				{
					$this->generateParseError("Unexpected end of data when short int data reading", $this->tlvDataPos);
					return null;
				}
				$a = unpack("@{$this->tlvDataPos}s/s1d", $this->tlvData);
				$this->tlvDataPos += 2;
				return $a["d"];
				
			case TLV::UINT_16:
				if ($this->tlvDataPos + 2 > $this->tlvDataLen)
				{
					$this->generateParseError("Unexpected end of data when short uint data reading", $this->tlvDataPos);
					return null;
				}
				$a = unpack("@{$this->tlvDataPos}s/v1d", $this->tlvData);
				$this->tlvDataPos += 2;
				return $a["d"];
				
			case TLV::INT_32:
				if ($this->tlvDataPos + 4 > $this->tlvDataLen)
				{
					$this->generateParseError("Unexpected end of data when int data reading", $this->tlvDataPos);
					return null;
				}
				$a = unpack("@{$this->tlvDataPos}s/l1d", $this->tlvData);
				$this->tlvDataPos += 4;
				return $a["d"];
				
			case TLV::UINT_32:
				if ($this->tlvDataPos + 4 > $this->tlvDataLen)
				{
					$this->generateParseError("Unexpected end of data when uint data reading", $this->tlvDataPos);
					return null;
				}
				$a = unpack("@{$this->tlvDataPos}s/V1d", $this->tlvData);
				$this->tlvDataPos += 4;
				return $a["d"];
				
			case TLV::STRING:
				return $this->readString();

			case TLV::ARRAY_VAL:
				return $this->readArray();

			case TLV::OBJECT_VAL:
				return $this->readObject();

			case TLV::END:
				return new TLVEndToken();

			case TLV::RECURSION:
				return "/*RECURSION*/";

			case TLV::UNAVAILABLE:
				return "/*UNAVAILABLE*/";

			case TLV::UNKNOWN:
				return null;
					
			default:
				$this->generateParseError("Unknown token type ({$tvlTokenType})", $this->tlvDataPos - 1);
				return null;
		}
	}

	/**
	 * Извлекает строку
	 */
	private function readString()
	{
		for ($pos = $this->tlvDataPos; $this->tlvDataPos < $this->tlvDataLen; $pos++)
		{
			if (ord($this->tlvData[$pos]) == 0)
			{
				break;
			}
		}
		
		$str = substr($this->tlvData, $this->tlvDataPos, $pos - $this->tlvDataPos);

		$this->tlvDataPos = $pos;
		
		if ($this->tlvDataPos + 1 > $this->tlvDataLen)
		{
			$this->generateParseError("Unexpected end of data when string reading", $this->tlvDataPos);
		}
		else
		{
			$this->tlvDataPos++;
		}
			
		return $str;
	}

	/**
	 * Читает данные последовательно по одному, пока не встретит TLVEndToken.
	 * Cкладывает в массив, который будет возвращен как одно значение.
	 */
	private function readArray()
	{
		$arr = array();
			
		while (true)
		{
			$item = $this->nextToken();
		
			if (is_a($item, "TLVEndToken"))
			{
				break;
			}
		
			$arr[] = $item;
		}
			
		return $arr;
	}

	/**
	 * Читает данные последовательно по паре ключ-значение, пока не встретит TLVEndToken в позиции ключа.
	 * Cкладывает в объект, который будет возвращен как одно значение.
	 */
	private function readObject()
	{
		$obj = new stdClass();
			
		while (true)
		{
			if ($this->tlvDataPos >= $this->tlvDataLen)
			{
				$this->generateParseError("Unexpected end of data when token type reading", $this->tlvDataPos);
				return null;
			}
						
			$a = unpack("@{$this->tlvDataPos}s/C1d", $this->tlvData);
			$tvlTokenType = $a["d"];
			
			if ($tvlTokenType == TLV::END)
			{
//print "End Object\n";	
				
				$this->tlvDataPos++;
				break;
			}
	
			$key = $this->readString();

//print "$key ...\n";	
			
			$val = $this->nextToken();
	
//print "$key = $val\n";
				
			if (is_a($val, "TLVEndToken"))
			{
				$this->generateParseError("Unexpected 'TLVEndToken' when object field value reading", $this->tlvDataPos - 1);
				$val = null;
			}
	
			$obj->$key = $val;
		}
				
		return $obj;
	}
}

/**
 * Класс обеспечивает сериализацию и десерлизацию для TLV-формата
 * 
 * @author Dmitriy Khaustov aka Dиmon
 */
class TLV
{
	const UNKNOWN = 255;
	
	const END = 0;
	
	const FALSE_VAL = 1;
	
	const TRUE_VAL = 2;
	
	const NULL_VAL = 3;
	
	const NAN = 4;
	
	const ZERO = 10;
	
	const INT_8 = 11;
	
	const UINT_8 = 12;
	
	const INT_16 = 13;
	
	const UINT_16 = 14;
	
	const INT_32 = 15;
	
	const UINT_32 = 16;
	
	const STRING = 20;
	
	const ARRAY_VAL = 30;
	
	const OBJECT_VAL = 40;
	
	const RECURSION = 50;
	
	const UNAVAILABLE = 60;
	
	/**
	 * Кодирует значение в TLV
	 */
	public static function encode($value)
	{
		$encoder = new TLVEncoder($value); 
		return $encoder->getData();
	}

	/**
	 * Декодирует бинарную строку
	 */
	public static function decode($encodedData, $strict = true)
	{
		$decoder = new TLVDecoder($encodedData, $strict);
		return $decoder->getValue();
	}
}

class TLVEndToken
{
}
