package cn.abellee.cniface.platform.service;

import cn.abellee.cniface.platform.domain.dto.GenderAgeRequestDTO;
import cn.abellee.cniface.platform.domain.dto.GenderAgeResult;

/**
 * @author abel
 * @date 2022/8/28 11:25 AM
 */
public interface IAttributeService {

    GenderAgeResult genderAge(GenderAgeRequestDTO genderAgeRequestDTO);
}
